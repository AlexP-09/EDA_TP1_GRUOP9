/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include <raylib.h>
#include <raymath.h>

/**
 * @brief Orbital body definition
 */
struct OrbitalBody
{
    const char *name;       // Name of the body
    float mass;             // Body mass [kg]
    float radius;           // Radius [m]
    Color color;            // Color (RGBA)
    Vector3 position;       // Position in 3 dimensions [m]
    Vector3 velocity;       // Velocity in 3 dimensions [m/s]
	char isAsteroid;        // Is this body an asteroid?
};

/**
 * @brief Orbital simulation definition
 */
struct OrbitalSim
{
    float timeStep;         // Timestep [s]
    float time;             // Time [s]
    int bodyNumber;         // Number of bodies
    OrbitalBody *bodies;    // Array of bodies
	int maxMassBodyIndex;   // Index of the body with the highest mass
};

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim);

#endif
