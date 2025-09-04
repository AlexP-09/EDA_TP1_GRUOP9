/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "orbitalSim.h"
#include "view.h"
#include <stdio.h>

#define FPS 60
#define DAYS_PER_SIM_SECOND 120

#define SECONDS_PER_DAY 86400
#define FAST_FORWARD_STEPS 10
#define FAST_FORWARD_KEY KEY_F
#define PAUSE_KEY KEY_P

int main()
{
    int fps = FPS;  // Frames per second

    // Simulation speed: 100 days per simulation second
    float timeMultiplier = DAYS_PER_SIM_SECOND * SECONDS_PER_DAY; 
    float timeStep = timeMultiplier / fps;

    OrbitalSim *sim = constructOrbitalSim(timeStep);
    View *view = constructView(fps);

    int i, simPaused = 0;
    while (isViewRendering(view))
    {
		// Pause or resume simulation if the key is pressed
        if(IsKeyPressed(PAUSE_KEY))
			simPaused = !simPaused;

        // Fast forward if the key is pressed
        for (i = 0; i < (IsKeyDown(FAST_FORWARD_KEY)? FAST_FORWARD_STEPS: 1) && !simPaused; i++)
            updateOrbitalSim(sim);

        renderView(view, sim);
    }

    destroyView(view);
    destroyOrbitalSim(sim);

    return 0;
}
