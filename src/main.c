#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define screenWidth 1200
#define screenHeight 960
#define mapWidth 24
#define mapHeight 24

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

Scene scene = {
    (Rectangle){0, 0, 240 * 4, 240 * 4},
    (Player){(Vector2){22, 11}, 0}};

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
}

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

    Vector2 mouse = GetMousePosition();
    Vector2 mosueGridPos = (Vector2){
        ((mouse.x - scene.boundary.x) / cellWidth),
        ((mouse.y - scene.boundary.y) / cellHeight)};
    circle(mosueGridPos, 3, (Color){0x00, 0xFF, 0x00, 0xFF});
    line(scene.player.pos, mosueGridPos, (Color){0x00, 0xFF, 0x00, 0xFF});

    // y = mx + c
    Vector2 p1 = scene.player.pos;
    Vector2 p2 = mosueGridPos;
    // p1.y = m * p1.x + c
    // p2.y = m * p2.x + c
    // m = (p2.y - p1.y) / (p2.x - p1.x)
    // c = p1.y - m * p1.x
    float m = (p2.y - p1.y) / (p2.x - p1.x);
    float c = p1.y - m * p1.x;

    // check for vertical line
    {
        int dir = 1;
        if (p2.x < p1.x)
        {
            dir = -1;
        }

        float x = p1.x;
        while (x < mapWidth)
        {
            float y = m * x + c;

            if (y <= 0 || y >= mapHeight - 1 || x <= 0 || x >= mapWidth - 1)
            {
                break;
            }

            int gridX = (int)x;
            int gridY = (int)y;
            if (dir == -1)
            {
                gridX = (int)x - 1;
            }

            if (worldMap[gridX][(int)y] != 0)
            {
                circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
            }
            x += dir;
        }
    }

    // check for horizontal line
    {
        int dir = 1;
        if (p2.y < p1.y)
        {
            dir = -1;
        }

        float y = p1.y;
        while (y < mapHeight)
        {
            float x = (y - c) / m;

            if (y <= 0 || y >= mapHeight - 1 || x <= 0 || x >= mapWidth - 1)
            {
                break;
            }

            int gridX = (int)x;
            int gridY = (int)y;
            if (dir == -1)
            {
                gridY = (int)y - 1;
            }

            if (worldMap[(int)x][gridY] != 0)
            {
                circle((Vector2){x, y}, 3, (Color){0xFF, 0x00, 0x00, 0xFF});
            }
            y += dir;
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

        BeginDrawing();
        ClearBackground((Color){0x18, 0x18, 0x18, 0xFF});
        miniMap();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}