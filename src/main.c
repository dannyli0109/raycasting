#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define P2 (PI / 2)
#define P3 (3 * PI / 2)

int screenWidth = 1200;
int screenHeight = 800;
int mapX = 8, mayY = 8, mapS = 30;
int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

Color colors[] = {
    (Color){0x00, 0x00, 0x00, 0xFF},
    (Color){0xFF, 0xFF, 0xFF, 0xFF},
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

Vector2 angleToVector(float angle)
{
    return (Vector2){cos(angle), sin(angle)};
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

Scene scene = {
    (Rectangle){0, 0, 240 * 4, 240 * 4},
    (Player){(Vector2){100, 100}, 0}};

float dist(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void miniMap()
{
    int x, y, xo, yo;
    for (y = 0; y < mayY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            DrawRectangle(x * mapS, y * mapS, mapS - 1, mapS - 1, colors[map[y * mapX + x]]);
        }
    }

    DrawCircle(scene.player.pos.x, scene.player.pos.y, 5, (Color){0xFF, 0x00, 0x00, 0xFF});
    // DrawLine(scene.player.pos.x * mapS, scene.player.pos.y * mapS, scene.player.pos.x * mapS + cos(scene.player.angle) * 20, scene.player.pos.y * mapS + sin(scene.player.angle) * 20, (Color){0xFF, 0x00, 0x00, 0xFF});
}

void drawRays3D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, dis;
    float fov = 90.0f;
    ra = scene.player.angle - DEG2RAD * fov / 2.0f;
    if (ra < 0)
    {
        ra += 2 * PI;
    }
    if (ra > 2 * PI)
    {
        ra -= 2 * PI;
    }
    int rays = fov * 4;
    for (r = 0; r < rays; ++r) {
        float shade = 1;
        dof = 0;
        float disH = 100000, disV = 100000, hx = scene.player.pos.x, hy = scene.player.pos.y, vx = scene.player.pos.x, vy = scene.player.pos.y;
        float disT = 0;
        float aTan = -1 / tan(ra);
        if (ra > PI) {
            ry = (((int)scene.player.pos.y / mapS) * mapS) - 0.0001;
            rx = (scene.player.pos.y - ry) * aTan + scene.player.pos.x;
            yo = -mapS;
            xo = -yo * aTan;
        }

        if (ra < PI) {
            ry = (((int)scene.player.pos.y / mapS) * mapS) + mapS;
            rx = (scene.player.pos.y - ry) * aTan + scene.player.pos.x;
            yo = mapS;
            xo = -yo * aTan;
        }

        if (ra == 0 || ra == PI) {
            rx = scene.player.pos.x;
            ry = scene.player.pos.y;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) / mapS;
            my = (int)(ry) / mapS;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapX && map[mp] == 1) {
                hx = rx; hy = ry; disH = dist(scene.player.pos.x, scene.player.pos.y, hx, hy, ra); dof = 8;
            } else {
                rx += xo; ry += yo; dof += 1;
            }
        }

        dof = 0;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) {
            rx = (((int)scene.player.pos.x / mapS) * mapS) - 0.0001;
            ry = (scene.player.pos.x - rx) * nTan + scene.player.pos.y;
            xo = -mapS;
            yo = -xo * nTan;
        }

        if (ra < P2 || ra > P3) {
            rx = (((int)scene.player.pos.x / mapS) * mapS) + mapS;
            ry = (scene.player.pos.x - rx) * nTan + scene.player.pos.y;
            xo = mapS;
            yo = -xo * nTan;
        }

        if (ra == 0 || ra == PI) {
            rx = scene.player.pos.x;
            ry = scene.player.pos.y;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) / mapS;
            my = (int)(ry) / mapS;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapX && map[mp] == 1) {
                vx = rx; vy = ry; disV = dist(scene.player.pos.x, scene.player.pos.y, vx, vy, ra); dof = 8;
            } else {
                rx += xo; ry += yo; dof += 1;
            }
        }

        if (disV < disH) {
            rx = vx;
            ry = vy;
            disT = disV;
            shade = 0.8;
        }
        

        if (disV > disH) {
            rx = hx;
            ry = hy;
            disT = disH;
            shade = 1;
        }


        DrawLine(scene.player.pos.x, scene.player.pos.y, rx, ry, (Color){0xFF, 0x00, 0x00, 0xFF});
        float ca = scene.player.angle - ra;
        if (ca < 0) {
            ca += 2.0f * PI;
        }
        if (ca > 2.0f * PI) {
            ca -= 2.0f * PI;
        }
        disT = disT * cos(ca);
        // --- draw 3D walls ---
        if (disT != 0) {
            float lineH =  (mapS * 800) / disT;
            if (lineH > screenHeight) {
                lineH = screenHeight;
            }
            float lineO = screenHeight / 2 - lineH / 2;
            float lineW = (float)screenWidth / (float)rays;
            int mp = (int)(ry / mapS) * mapX + (int)(rx / mapS);
            Color c = colors[map[mp]];
            c.r = c.r * shade;
            c.g = c.g * shade;
            c.b = c.b * shade;
    

            DrawRectangle(r * lineW, lineO, (int)lineW + 1, lineH, c );
        }


        ra += DEG2RAD * fov / rays;
        if (ra < 0) {
            ra += 2 * PI;
        }
        if (ra > 2 * PI) {
            ra -= 2 * PI;
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
            scene.player.pos = vectorAdd(scene.player.pos, vectorMul(angleToVector(scene.player.angle), dt * mapS));
        }
        if (IsKeyDown(KEY_S))
        {
            scene.player.pos = vectorSub(scene.player.pos, vectorMul(angleToVector(scene.player.angle), dt * mapS));
        }
        if (IsKeyDown(KEY_A))
        {
            scene.player.angle -= PI * dt;
            if (scene.player.angle < 0)
            {
                scene.player.angle += 2 * PI;
            }
        }
        if (IsKeyDown(KEY_D))
        {
            scene.player.angle += PI * dt;
            if (scene.player.angle > 2 * PI)
            {
                scene.player.angle -= 2 * PI;
            }
        }

        BeginDrawing();
        ClearBackground((Color){0x18, 0x18, 0x18, 0xFF});
        // render();
        miniMap();
        drawRays3D();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}