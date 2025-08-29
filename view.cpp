/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <time.h>

#include "view.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SOLARSYSTEM_BODYNUM 9

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char *getISODate(float timestamp)
{
    // Timestamp epoch: 1/1/2022
    struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

    // Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
    time_t unixEpoch = mktime(&unichEpochTM);
    time_t unixTimestamp = unixEpoch + (time_t)timestamp;

    // Returns ISO date
    struct tm *localTM = localtime(&unixTimestamp);
    return TextFormat("%04d-%02d-%02d",
                      1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int fps)
{
    View *view = new View();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
    SetTargetFPS(fps);
    DisableCursor();

    view->camera.position = {10.0f, 10.0f, 10.0f};
    view->camera.target = {0.0f, 0.0f, 0.0f};
    view->camera.up = {0.0f, 1.0f, 0.0f};
    view->camera.fovy = 45.0f;
    view->camera.projection = CAMERA_PERSPECTIVE;

    return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view)
{
    CloseWindow();

    delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(View *view)
{
    return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(View *view, OrbitalSim *sim)
{
    UpdateCamera(&view->camera, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode3D(view->camera);

    int i;
    for (i = 0; i < sim->bodyNumber; i++)
    {
        OrbitalBody* body = &sim->bodies[i];
        DrawSphere(body->position * 1E-11F, 0.005f * logf(body->radius), body->color);
        DrawPoint3D(body->position * 1E-11F, body->color);
    }

    DrawGrid(10, 10.0f);
    EndMode3D();

    DrawText("Orbital Simulation", 10, 10, 20, RAYWHITE);
    DrawFPS(10, 30);


    for (i = 0; i < SOLARSYSTEM_BODYNUM ; i++)
    {
        OrbitalBody* body = &sim->bodies[i];
        Vector3 pos3D = { body->position.x*1E-11f, 
                          body->position.y* 1E-11f + 0.005f * logf(body->radius) * 1.1f,
                          body->position.z * 1E-11f};
        Vector3 camDir = Vector3Normalize(Vector3Subtract(view->camera.target, view->camera.position));     
        Vector3 bodyDir = Vector3Normalize(Vector3Subtract(pos3D, view->camera.position));                  
        float prod = Vector3DotProduct(camDir, bodyDir);        // Veo si el objeto está en frente de la cámara o no

        if (prod > 0.1f)
        {
            Vector2 pos2D = GetWorldToScreen(pos3D, view->camera);
            DrawText(TextFormat("%s", body->name), (int)pos2D.x, (int)pos2D.y + 0.5f, 15, WHITE);
        }
    }
    EndDrawing();
}
