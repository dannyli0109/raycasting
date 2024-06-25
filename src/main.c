#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define screenWidth 1200
#define screenHeight 960
#define mapWidth 24
#define mapHeight 24
#define ESP 0.0000001
#define rayStep 0.01
#define stripWidth 1

int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

Color colors[] = {
    (Color){0x00, 0x00, 0x00, 0xFF},
    (Color){0xFF, 0x00, 0x00, 0xFF},
    (Color){0x00, 0xFF, 0x00, 0xFF},
    (Color){0x00, 0x00, 0xFF, 0xFF},
    (Color){0xFF, 0xFF, 0x00, 0xFF},
    (Color){0xFF, 0x00, 0xFF, 0xFF}};

typedef struct Player
{
    Vector2 pos;
    float angle;
} Player;

typedef struct Scene
{
    Rectangle boundary;
    Player player;
} Scene;

Vector2 vectorAdd(Vector2 a, Vector2 b)
{
    return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 vectorSub(Vector2 a, Vector2 b)
{
    return (Vector2){a.x - b.x, a.y - b.y};
}

Vector2 vectorMul(Vector2 a, float b)
{
    return (Vector2){a.x * b, a.y * b};
}

Vector2 vectorDiv(Vector2 a, float b)
{
    return (Vector2){a.x / b, a.y / b};
}

float vectorMag(Vector2 a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

Vector2 vectorNorm(Vector2 a)
{
    return vectorDiv(a, vectorMag(a));
}

float vectorDot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 angleToVectorDeg(float angle)
{
    return (Vector2){cos(angle * DEG2RAD), sin(angle * DEG2RAD)};
}

Vector2 rotateVectorDeg(Vector2 a, float angle)
{
    return (Vector2){
        a.x * cos(angle * DEG2RAD) - a.y * sin(angle * DEG2RAD),
        a.x * sin(angle * DEG2RAD) + a.y * cos(angle * DEG2RAD)};
}

Vector2 vectorLerp(Vector2 a, Vector2 b, float t)
{
    return vectorAdd(a, vectorMul(vectorSub(b, a), t));
}

float vectorDistance(Vector2 a, Vector2 b)
{
    return vectorMag(vectorSub(a, b));
}

int sign(float x)
{
    return (x > 0) - (x < 0);
}

Vector2 hittingCell(Vector2 p1, Vector2 p2)
{
    Vector2 d = vectorSub(p2, p1);
    return (Vector2){floorf(p2.x + sign(d.x) * ESP), floorf(p2.y + sign(d.y) * ESP)};
}

Scene scene = {
    (Rectangle){0, 0, 240, 240},
    (Player){(Vector2){15, 11}, -45}};

void rect(Rectangle rect, Color color)
{
    // rect is from 0 to mapWidth and mapHeight
    // cell width and height
    float cellWidth = scene.boundary.width / mapWidth;
    float cellHeight = scene.boundary.height / mapHeight;

    // draw the rectangle
    DrawRectangle((int)(rect.x * cellWidth + scene.boundary.x), (int)(rect.y * cellHeight + scene.boundary.y), (int)(rect.width * cellWidth), (int)(rect.height * cellHeight), color);
}

void line(Vector2 start, Vector2 end, Color color)
{
    // cell width and height
    float cellWidth = scene.boundary.width / mapWidth;
    float cellHeight = scene.boundary.height / mapHeight;

    // draw the line
    DrawLine((int)(start.x * cellWidth + scene.boundary.x), (int)(start.y * cellHeight + scene.boundary.y), (int)(end.x * cellWidth + scene.boundary.x), (int)(end.y * cellHeight + scene.boundary.y), color);
}

void circle(Vector2 center, float radius, Color color)
{
    // cell width and height
    float cellWidth = scene.boundary.width / mapWidth;
    float cellHeight = scene.boundary.height / mapHeight;

    // draw the circle
    DrawCircle((int)(center.x * cellWidth + scene.boundary.x), (int)(center.y * cellHeight + scene.boundary.y), radius, color);
};

void miniMap()
{
    float cellWidth = scene.boundary.width / mapWidth;
    float cellHeight = scene.boundary.height / mapHeight;

    for (int i = 0; i < mapWidth; i++)
    {
        for (int j = 0; j < mapHeight; j++)
        {
            if (worldMap[i][j] == 0)
                continue;
            rect((Rectangle){i, j, 1, 1}, colors[worldMap[i][j]]);
        }
    }

    for (int i = 0; i < mapWidth; i++)
    {
        line((Vector2){i, 0}, (Vector2){i, mapHeight}, (Color){0x00, 0x00, 0x00, 0xFF});
    }

    for (int i = 0; i < mapHeight; i++)
    {
        line((Vector2){0, i}, (Vector2){mapWidth, i}, (Color){0x00, 0x00, 0x00, 0xFF});
    }

    circle(scene.player.pos, 3, (Color){0x00, 0xFF, 0x00, 0xFF});
    Vector2 dir = angleToVectorDeg(scene.player.angle);
    float fov = 90;
    float halfFov = fov / 2;
    // sin(halfFov) = halfWidth / distance
    float halfWidth = tan(halfFov * DEG2RAD);
    Vector2 pp2 = vectorAdd(scene.player.pos, dir);
    Vector2 left = vectorAdd(pp2, vectorMul(rotateVectorDeg(dir, -90), halfWidth));
    Vector2 right = vectorAdd(pp2, vectorMul(rotateVectorDeg(dir, 90), halfWidth));

    line(scene.player.pos, pp2, (Color){0x00, 0xFF, 0x00, 0xFF});
    line(scene.player.pos, left, (Color){0x00, 0xFF, 0x00, 0xFF});
    line(scene.player.pos, right, (Color){0x00, 0xFF, 0x00, 0xFF});

    // line(scene.player.pos, vectorAdd(scene.player.pos, right), (Color){0x00, 0xFF, 0x00, 0xFF});
    line(left, right, (Color){0x00, 0xFF, 0x00, 0xFF});

    Vector2 plDir = vectorNorm(vectorSub(left, scene.player.pos));
    Vector2 prDir = vectorNorm(vectorSub(right, scene.player.pos));

    Vector2 mouse = GetMousePosition();
    Vector2 mosueGridPos = (Vector2){
        ((mouse.x - scene.boundary.x) / cellWidth),
        ((mouse.y - scene.boundary.y) / cellHeight)};
    circle(mosueGridPos, 3, (Color){0x00, 0xFF, 0x00, 0xFF});

    line(scene.player.pos, mosueGridPos, (Color){0x00, 0xFF, 0x00, 0xFF});

    Vector2 pl = left;
    Vector2 pr = right;

    // for (int i = 0; i < screenWidth; i++)
    // {
    //     float t = (float)i / screenWidth;
    //     Vector2 p = vectorLerp(pl, pr, t);
    //     // line(scene.player.pos, p, (Color){0xFF, 0xFF, 0xFF, 0xFF});
    //     // p1.y = m * p1.x + c
    //     // p2.y = m * p2.x + c
    //     // m = (p2.y - p1.y) / (p2.x - p1.x)
    //     // c = p1.y - m * p1.x
    //     Vector2 p1 = scene.player.pos;
    //     Vector2 p2 = p;

    //     float m = (p2.y - p1.y) / (p2.x - p1.x);
    //     float c = p1.y - m * p1.x;

    //     Vector2 blockY = {0, 0};
    //     bool isBlockY = false;

    //     // check for vertical line
    //     {
    //         float dir = 0.01;
    //         if (p2.x < p1.x)
    //         {
    //             dir = -0.01;
    //         }

    //         float x = p1.x;
    //         while (x < mapWidth)
    //         {
    //             float y = m * x + c;

    //             if (y <= 0 || y >= mapHeight - 1 || x <= 0 || x >= mapWidth - 1)
    //             {
    //                 break;
    //             }

    //             Vector2 cell = hittingCell(p1, (Vector2){x, y});

    //             if (worldMap[(int)cell.x][(int)cell.y] != 0)
    //             {
    //                 // circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
    //                 blockY = (Vector2){x, y};
    //                 isBlockY = true;
    //                 break;
    //             }
    //             x += dir;
    //         }
    //     }

    //     Vector2 blockX = {0, 0};
    //     bool isBlockX = false;

    //     // check for horizontal line
    //     {
    //         float dir = 0.01;
    //         if (p2.y < p1.y)
    //         {
    //             dir = -0.01;
    //         }

    //         float y = p1.y;
    //         while (y < mapHeight)
    //         {
    //             float x = (y - c) / m;

    //             if (y <= 0 || y >= mapHeight - 1 || x <= 0 || x >= mapWidth - 1)
    //             {
    //                 break;
    //             }

    //             Vector2 cell = hittingCell(p1, (Vector2){x, y});

    //             if (worldMap[(int)cell.x][(int)cell.y] != 0)
    //             {
    //                 // circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
    //                 blockX = (Vector2){x, y};
    //                 isBlockX = true;
    //                 break;
    //             }
    //             y += dir;
    //         }
    //     }

    //     Vector2 block = {-1, -1};

    //     if (isBlockX && isBlockY)
    //     {
    //         float distX = vectorMag(vectorSub(scene.player.pos, blockX));
    //         float distY = vectorMag(vectorSub(scene.player.pos, blockY));
    //         if (distX < distY)
    //         {
    //             block = blockX;
    //         }
    //         else
    //         {
    //             block = blockY;
    //         }
    //     }
    //     else if (isBlockX)
    //     {
    //         block = blockX;
    //     }
    //     else if (isBlockY)
    //     {
    //         block = blockY;
    //     }

    //     if (block.x != -1 && block.y != -1)
    //     {
    //         circle(block, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
    //     }
    // }
}

void render()
{
    Vector2 dir = angleToVectorDeg(scene.player.angle);
    float fov = 90;
    float halfFov = fov / 2;
    // sin(halfFov) = halfWidth / distance
    float halfWidth = tan(halfFov * DEG2RAD);
    Vector2 pp2 = vectorAdd(scene.player.pos, dir);
    Vector2 left = vectorAdd(pp2, vectorMul(rotateVectorDeg(dir, -90), halfWidth));
    Vector2 right = vectorAdd(pp2, vectorMul(rotateVectorDeg(dir, 90), halfWidth));

    // y = mx + c
    Vector2 pl = left;
    Vector2 pr = right;

    int w = screenWidth / stripWidth;
    for (int x = 0; x < w; x++)
    {
        float t = (float)x / w;
        Vector2 p = vectorLerp(pl, pr, t);
        // line(scene.player.pos, p, (Color){0xFF, 0xFF, 0xFF, 0xFF});
        // p1.y = m * p1.x + c
        // p2.y = m * p2.x + c
        // m = (p2.y - p1.y) / (p2.x - p1.x)
        // c = p1.y - m * p1.x
        Vector2 p1 = scene.player.pos;
        Vector2 p2 = p;

        float m = (p2.y - p1.y) / (p2.x - p1.x);
        float c = p1.y - m * p1.x;

        Vector2 blockY = {0, 0};
        bool isBlockY = false;

        // check for vertical line
        {
            float dir = rayStep;
            if (p2.x < p1.x)
            {
                dir = -rayStep;
            }

            float x = p1.x;
            while (x < mapWidth)
            {
                float y = m * x + c;

                if (y <= 0 || y >= mapHeight || x <= 0 || x >= mapWidth)
                {
                    break;
                }

                Vector2 cell = hittingCell(p1, (Vector2){x, y});

                if (worldMap[(int)cell.x][(int)cell.y] != 0)
                {
                    // circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
                    blockY = (Vector2){x, y};
                    isBlockY = true;
                    break;
                }
                x += dir;
            }
        }

        Vector2 blockX = {0, 0};
        bool isBlockX = false;

        // check for horizontal line
        {
            float dir = rayStep;
            if (p2.y < p1.y)
            {
                dir = -rayStep;
            }

            float y = p1.y;
            while (y < mapHeight)
            {
                float x = (y - c) / m;

                if (y <= 0 || y >= mapHeight || x <= 0 || x >= mapWidth)
                {
                    break;
                }

                Vector2 cell = hittingCell(p1, (Vector2){x, y});

                if (worldMap[(int)cell.x][(int)cell.y] != 0)
                {
                    // circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
                    blockX = (Vector2){x, y};
                    isBlockX = true;
                    break;
                }
                y += dir;
            }
        }

        Vector2 block = {-1, -1};

        if (isBlockX && isBlockY)
        {
            float distX = vectorMag(vectorSub(scene.player.pos, blockX));
            float distY = vectorMag(vectorSub(scene.player.pos, blockY));
            if (distX < distY)
            {
                block = blockX;
            }
            else
            {
                block = blockY;
            }
        }
        else if (isBlockX)
        {
            block = blockX;
        }
        else if (isBlockY)
        {
            block = blockY;
        }

        if (block.x != -1 && block.y != -1)
        {
            // dot product of the vector from player to block and the direction vector
            float dist = vectorDot(vectorSub(scene.player.pos, block), dir);
            float wallHeight = screenHeight / dist;
            float wallStart = screenHeight / 2 - wallHeight / 2;
            float wallEnd = screenHeight / 2 + wallHeight / 2;

            Color color = colors[worldMap[(int)block.x][(int)block.y]];
            // DrawLine(x * stripWidth, wallStart, x * stripWidth, wallEnd, color);

            // draw rectangle
            // Rectangle rect = (Rectangle){x * stripWidth, wallStart, stripWidth, wallHeight};
            for (int i = 0; i < stripWidth; i++)
            {
                // britness based on distance
                int brightness = 255 / dist;

                DrawLine(x * stripWidth + i, wallStart, x * stripWidth + i, wallEnd, color);
            }
        }
    }
}

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Raycasting");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        float dt = GetFrameTime();
        if (IsKeyDown(KEY_W))
        {
            scene.player.pos = vectorAdd(scene.player.pos, vectorMul(angleToVectorDeg(scene.player.angle), dt * 4));
        }
        if (IsKeyDown(KEY_S))
        {
            scene.player.pos = vectorSub(scene.player.pos, vectorMul(angleToVectorDeg(scene.player.angle), dt * 4));
        }
        if (IsKeyDown(KEY_A))
        {
            scene.player.angle -= 120 * dt;
        }
        if (IsKeyDown(KEY_D))
        {
            scene.player.angle += 120 * dt;
        }

        BeginDrawing();
        ClearBackground((Color){0x18, 0x18, 0x18, 0xFF});
        render();
        miniMap();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}