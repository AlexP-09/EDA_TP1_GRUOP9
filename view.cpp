/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <time.h>
#include <stdio.h>
#include "view.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SOLARSYSTEM_BODYNUM 9

#define MAX_WIRES 6
#define MAX_SPHERE_RENDER_DISTANCE 4.0f

#define POSITION_SCALE 1E-11f
#define RADIUS_SCALE(r) (0.005f * logf(r))

#define VIEW_MARGIN 100

static int isEntityInView(View* view, Vector3 position);

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

	// Set up camera
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
    int i;

	// Set up scale factors once for every planet and the asteroids (every asteroid has the same size)
	static bool scaleSet = false;
    static float scaledRadius[SOLARSYSTEM_BODYNUM + 1];

    if(!scaleSet)
    {
        for (i = 0; i < SOLARSYSTEM_BODYNUM + 1; i++)
        {
            scaledRadius[i] = RADIUS_SCALE(sim->bodies[i].radius);
        }
		scaleSet = true;
    }

    UpdateCamera(&view->camera, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode3D(view->camera);

    for (i = 0; i < sim->bodyNumber; i++)
    {
        OrbitalBody* body = &sim->bodies[i];

		float distToCam = Vector3Length(Vector3Subtract(body->position * POSITION_SCALE, view->camera.position));
		int radiusIndex = i >= SOLARSYSTEM_BODYNUM ? SOLARSYSTEM_BODYNUM : i;
        
		// Skip entities not in view
        if(!isEntityInView(view, Vector3Scale(body->position, POSITION_SCALE)))
			continue;

		// Draws a sphere for big bodies or asteroids close to the camera, wireframe for asteroids further away
        if(!body->isAsteroid || distToCam < MAX_SPHERE_RENDER_DISTANCE)
            DrawSphere(body->position * POSITION_SCALE, scaledRadius[radiusIndex], body->color);
        else
        {
			// Draw wireframe for asteroids depending on distance (linear function)
			int wiresToDraw = (int) fmaxf(MAX_WIRES - distToCam / MAX_SPHERE_RENDER_DISTANCE, 1.0f);
            
            DrawSphereWires(body->position * POSITION_SCALE, scaledRadius[radiusIndex], wiresToDraw, wiresToDraw, body->color);
        }

        // Always draw a point
        DrawPoint3D(body->position * POSITION_SCALE, body->color);
    }

    DrawGrid(10, 10.0f);
    EndMode3D();

	// 2D overlay with sim information
    DrawText("Orbital Simulation", 10, 10, 20, RAYWHITE);
    DrawFPS(10, 30);
	DrawText(getISODate(sim->time), 10, 50, 20, RAYWHITE);
    
	// Draw names of big bodies
    for(i = 0; i < SOLARSYSTEM_BODYNUM ; i++)
    {
        OrbitalBody* body = &sim->bodies[i];
        Vector3 pos3D = { body->position.x * POSITION_SCALE, 
                          body->position.y * POSITION_SCALE + (scaledRadius[i >= SOLARSYSTEM_BODYNUM? SOLARSYSTEM_BODYNUM: i]) * 1.2f,
                          body->position.z * POSITION_SCALE
        };
  
        if (isEntityInView(view, Vector3Scale(body->position, POSITION_SCALE)))
        {
            Vector2 pos2D = GetWorldToScreen(pos3D, view->camera);
            DrawText(TextFormat("%s", body->name), (int)pos2D.x, (int)pos2D.y + 0.5f, 15, WHITE);
        }
    }

    EndDrawing();
}

static int isEntityInView(View* view, Vector3 position)
{
	// Get the screen position of the entity
    Vector2 screenPos = GetWorldToScreen(position, view->camera);

	// Check if the entity is in front of the camera
	Vector3 toEntity = Vector3Normalize(Vector3Subtract(position, view->camera.position));
	Vector3 forward = Vector3Normalize(Vector3Subtract(view->camera.target, view->camera.position));
    float dotProduct = Vector3DotProduct(forward, toEntity);
    if(dotProduct < 0.01f)
		return 0;

	// Check if the position is within the screen bounds plus a margin
    return (screenPos.x >= -VIEW_MARGIN && screenPos.x <= WINDOW_WIDTH + VIEW_MARGIN &&
        screenPos.y >= -VIEW_MARGIN && screenPos.y <= WINDOW_HEIGHT + VIEW_MARGIN);
}