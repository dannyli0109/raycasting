#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

const int gridRows = 20;
const int gridCols = 20;
const float clippingPlane = 1;
const float fov = 90;

typedef struct Player
{
    Vector2 pos;
    float angle;
} Player;

typedef struct Scene
{
    Player player;
    int grid[gridRows][gridCols];
} Scene;

typedef struct FovRange
{
    Vector2 pl;
    Vector2 pr
} FovRange;

Scene scene = {
    (Player){(Vector2){2, 3}, 0},
    {0}};

Color colors[] = {
    (Color){0x3D, 0x3D, 0x3D, 0xFF},
    (Color){0xFF, 0x00, 0x00, 0xFF},
    (Color){0x00, 0xFF, 0x00, 0xFF},
    (Color){0x00, 0x00, 0xFF, 0xFF}};

typedef struct BlockInfo
{
    Vector2 pos;
    Vector2 gridPos;
} BlockInfo;

Vector2
getGridCoords(Rectangle bound, Vector2 pos)
{
    float gridWidth = bound.width / gridRows;
    float gridHeight = bound.height / gridCols;
    return (Vector2){(int)((pos.x - bound.x) / gridWidth), (int)((pos.y - bound.y) / gridHeight)};
}

Vector2 gridCoordsToRectPos(Rectangle bound, Vector2 gridPos)
{
    float gridWidth = bound.width / gridRows;
    float gridHeight = bound.height / gridCols;
    return (Vector2){gridPos.x * gridWidth + bound.x + gridWidth / 2, gridPos.y * gridHeight + bound.y + gridHeight / 2};
}

Vector2 getVectorFromAngleDeg(float angle)
{
    return (Vector2){cos(angle * DEG2RAD), sin(angle * DEG2RAD)};
};

Vector2 vectorAdd(Vector2 v1, Vector2 v2)
{
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

Vector2 vectorSub(Vector2 v1, Vector2 v2)
{
    return (Vector2){v1.x - v2.x, v1.y - v2.y};
}

Vector2 vectorMult(Vector2 v, float scalar)
{
    return (Vector2){v.x * scalar, v.y * scalar};
}

Vector2 vectorLerp(Vector2 v1, Vector2 v2, float t)
{
    return (Vector2){v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t};
}

float dot(Vector2 v1, Vector2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float distance(Vector2 p1, Vector2 p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

bool isOutOfBounds(Rectangle bound, float x, float y)
{
    return x < bound.x || x > (bound.width + bound.x) || y < bound.y || y > (bound.height + bound.y);
}

FovRange getFovRange(Rectangle bound, Vector2 pos, float angle, float fov, float clippingPlane)
{
    Vector2 p1 = gridCoordsToRectPos(bound, pos);
    Vector2 p2 = vectorAdd(p1, vectorMult(getVectorFromAngleDeg(angle), clippingPlane));
    float l = tanf(fov / 2 * DEG2RAD) * clippingPlane;
    Vector2 pl = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(angle - 90), l));
    Vector2 pr = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(angle + 90), l));

    return (FovRange){pl, pr};
}

BlockInfo getClosestIntersection(Rectangle bound, Vector2 start, Vector2 p2, float m, float b, bool checkX)
{
    float gridWidth = bound.width / gridRows;
    float gridHeight = bound.height / gridCols;

    Vector2 gridStart = {
        ((int)((start.x - bound.x) / gridWidth)) * gridWidth + bound.x,
        ((int)((start.y - bound.y) / gridHeight)) * gridHeight + bound.y};

    int dir = (checkX ? (start.x > p2.x) : (start.y > p2.y)) ? -1 : 1;
    int i = checkX ? gridStart.x : gridStart.y;

    while ((checkX && ((dir == 1 && i <= bound.width + bound.x) || (dir == -1 && i >= bound.x))) ||
           (!checkX && ((dir == 1 && i <= bound.height + bound.y) || (dir == -1 && i >= bound.y))))
    {

        float coord = checkX ? m * i + b : (i - b) / m;
        float x = checkX ? i : coord;
        float y = checkX ? coord : i;

        if (isOutOfBounds(bound, x, y))
        {
            break;
        }

        if (checkX && dir == 1 && i < start.x)
        {
            i += gridWidth * dir;
            continue;
        }
        else if (!checkX && dir == 1 && i < start.y)
        {
            i += gridHeight * dir;
            continue;
        }

        Vector2 gridPos = getGridCoords(bound, (Vector2){x, y});

        if (gridPos.x <= 0 || gridPos.x >= gridRows || gridPos.y <= 0 || gridPos.y >= gridCols)
        {
            i += gridHeight * dir;
            continue;
        }

        if (dir == -1)
        {
            if (checkX)
            {
                gridPos.x -= 1;
            }
            else
            {
                gridPos.y -= 1;
            }
        }

        if (scene.grid[(int)gridPos.x][(int)gridPos.y] != 0)
        {
            return (BlockInfo){(Vector2){x, y}, gridPos};
        }

        i += (checkX ? gridWidth : gridHeight) * dir;
    }

    // return intersection with the wall
    // if (checkX)
    // {
    //     float x = (dir == 1 ? bound.width + bound.x : bound.x);
    //     return (Vector2){x, m * x + b};
    // }
    // else
    // {
    //     float y = (dir == 1 ? bound.height + bound.y : bound.y);
    //     return (Vector2){(y - b) / m, y};
    // }
    return (BlockInfo){(Vector2){-1, -1}, (Vector2){-1, -1}};
}

void miniMap(Rectangle rect)
{
    const int gridWidth = rect.width / gridRows;
    const int gridHeight = rect.height / gridCols;
    for (int i = 0; i < gridRows; i++)
    {
        for (int j = 0; j < gridCols; j++)
        {
            Rectangle rec = {i * rect.width / gridRows + rect.x, j * rect.height / gridCols + rect.y, gridWidth, gridHeight};

            DrawRectangleRec(rec, colors[scene.grid[i][j]]);

            DrawRectangleLinesEx(rec, 1, (Color){0x2D, 0x2D, 0x2D, 0xFF});
        }
    }

    Vector2 p1 = gridCoordsToRectPos(rect, scene.player.pos);

    Vector2 p2 = vectorAdd(p1, vectorMult(getVectorFromAngleDeg(scene.player.angle), clippingPlane * gridWidth));
    float l = tanf(fov / 2 * DEG2RAD) * clippingPlane * gridWidth;
    Vector2 pl = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(scene.player.angle - 90), l));
    Vector2 pr = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(scene.player.angle + 90), l));

    DrawLineV(p1, pl, (Color){0x00, 0xFF, 0x00, 0xFF});
    DrawLineV(p1, pr, (Color){0x00, 0xFF, 0x00, 0xFF});
    DrawLineV(pl, pr, (Color){0x00, 0xFF, 0x00, 0xFF});

    for (int x = 0; x < 1200; x++)
    {
        float t = (float)x / 1200;
        Vector2 p = vectorLerp(pl, pr, t);
        const float m = (p.y - p1.y) / (p.x - p1.x);
        const float b = p1.y - m * p1.x;
        Vector2 p1 = gridCoordsToRectPos(rect, scene.player.pos);

        BlockInfo blockY = getClosestIntersection(rect, p1, p, m, b, true);
        BlockInfo blockX = getClosestIntersection(rect, p1, p, m, b, false);

        BlockInfo block = (distance(p1, blockY.pos) < distance(p1, blockX.pos)) ? blockY : blockX;

        if (block.gridPos.x < 0 && block.gridPos.y < 0)
        {
            continue;
        }

        DrawCircleV(block.pos, 1, (Color){0xFF, 0x00, 0x00, 0xFF});
    }
}

void render(Rectangle bound)
{
    // Calculate the starting position of the player in the grid coordinates
    Vector2 p1 = gridCoordsToRectPos(bound, scene.player.pos);
    const int gridWidth = bound.width / gridRows;
    const int gridHeight = bound.height / gridCols;

    // Calculate the forward direction vector based on the player's angle
    Vector2 p2 = vectorAdd(p1, vectorMult(getVectorFromAngleDeg(scene.player.angle), clippingPlane * gridWidth));

    // Calculate the left and right boundaries of the FOV
    float l = tanf(fov / 2 * DEG2RAD) * clippingPlane * gridWidth;
    Vector2 pl = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(scene.player.angle - 90), l));
    Vector2 pr = vectorAdd(p2, vectorMult(getVectorFromAngleDeg(scene.player.angle + 90), l));

    // Loop through each vertical line of the screen
    for (int x = 0; x < 1200; x++)
    {
        // Interpolate between the left and right boundaries of the FOV
        float t = (float)x / 1200;
        Vector2 p = vectorLerp(pl, pr, t);

        // Calculate the slope and intercept for the line equation
        const float m = (p.y - p1.y) / (p.x - p1.x);
        const float b = p1.y - m * p1.x;

        // Recalculate the player's position in grid coordinates
        Vector2 p1 = gridCoordsToRectPos(bound, scene.player.pos);

        // Find the closest intersection points
        BlockInfo blockY = getClosestIntersection(bound, p1, p, m, b, true);
        BlockInfo blockX = getClosestIntersection(bound, p1, p, m, b, false);
        BlockInfo block = (distance(p1, blockY.pos) < distance(p1, blockX.pos)) ? blockY : blockX;

        // Skip if the intersection point is invalid
        if (block.pos.x < 0 && block.pos.y < 0)
        {
            continue;
        }

        // Calculate the grid cell coordinates of the intersection point

        // Calculate the distance vector and the height of the wall slice
        Vector2 v = vectorSub(block.pos, p1);
        Vector2 d = getVectorFromAngleDeg(scene.player.angle);
        float distance = dot(v, d);

        // Normalize the distance to prevent too small height values
        float h = 10000 / (distance + 0.0001f); // Added a small value to avoid division by zero

        // Draw the vertical line representing the wall slice
        DrawLineV((Vector2){x, 400 - h / 2}, (Vector2){x, 400 + h / 2}, colors[scene.grid[(int)block.gridPos.x][(int)block.gridPos.y]]);
    }
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Raycasting");

    for (int i = 0; i < gridRows; i++)
    {
        for (int j = 0; j < gridCols; j++)
        {
            scene.grid[i][j] = 0;
        }
    }

    scene.grid[1][1] = 1;
    scene.grid[2][1] = 2;
    scene.grid[6][6] = 1;
    scene.grid[6][7] = 3;

    SetTargetFPS(60);

    float gridWidth = screenWidth / gridRows;
    float gridHeight = screenHeight / gridCols;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_A))
        {
            scene.player.angle -= 1;
        }

        if (IsKeyDown(KEY_D))
        {
            scene.player.angle += 1;
        }

        if (IsKeyDown(KEY_W))
        {
            Vector2 newPos = vectorAdd(scene.player.pos, vectorMult(getVectorFromAngleDeg(scene.player.angle), 0.1));
            // Vector2 gridPos = getGridCoords((Rectangle){0, 0, 200, 200}, newPos);
            // make sure the player is not moving into a wall
            // printf("gridPos: %f, %f\n", (int)newPos.x, (int)newPos.y);
            // if (scene.grid[(int)newPos.x][(int)newPos.y] != 0)
            // {
            //     newPos = scene.player.pos;
            // }

            if (newPos.x < -0.5 || newPos.x >= gridRows - 0.5 || newPos.y < -0.5 || newPos.y >= gridCols - 0.5)
            {
                newPos = scene.player.pos;
            }

            scene.player.pos = newPos;
        }

        if (IsKeyDown(KEY_S))
        {
            Vector2 newPos = vectorAdd(scene.player.pos, vectorMult(getVectorFromAngleDeg(scene.player.angle), -0.1));
            if (newPos.x < -0.5 || newPos.x >= gridRows - 0.5 || newPos.y < -0.5 || newPos.y >= gridCols - 0.5)
            {
                newPos = scene.player.pos;
            }

            scene.player.pos = newPos;
        }

        BeginDrawing();

        ClearBackground((Color){0x18, 0x18, 0x18, 0xFF});
        // Draw grid
        render((Rectangle){0, 0, 200, 200});
        miniMap((Rectangle){0, 0, 200, 200});
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}