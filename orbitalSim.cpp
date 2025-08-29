/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "orbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

#define ASTEROIDS_NUMBER 1000
#define MASS_THRESHOLD 1E20F
#define MIN_DISTANCE 1E7F

static Vector3 getForce(OrbitalBody *body1, OrbitalBody *body2);
static Vector3 getAcceleration(OrbitalBody *body, OrbitalSim *sim);
static Vector3 getVelocity(OrbitalBody body, Vector3 acceleration, float timeStep);
static Vector3 getPosition(OrbitalBody body, float timeStep);

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
	// Easter egg: uncomment to align all asteroids in a straight line
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

	// Asteroid properties
	body->name = "";
    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->position = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
	body->isAsteroid = 1;
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim *constructOrbitalSim(float timeStep)
{
    if(timeStep <= 0)
        return NULL;
    
    OrbitalSim *sim = (OrbitalSim *) malloc(sizeof(OrbitalSim));
    if(!sim)
        return NULL;

	sim->bodyNumber = SOLARSYSTEM_BODYNUM + ASTEROIDS_NUMBER;

    sim->bodies = (OrbitalBody*) malloc(sizeof(OrbitalBody) * sim->bodyNumber);
    if (!sim->bodies)
    {
       free(sim);
       return NULL;
    }
        
    sim->timeStep = timeStep;
    sim->time = 0;
    
    int i;
    for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        sim->bodies[i].name = solarSystem[i].name;
        sim->bodies[i].mass = solarSystem[i].mass;
        sim->bodies[i].radius = solarSystem[i].radius;
        sim->bodies[i].color = solarSystem[i].color;
        sim->bodies[i].position = solarSystem[i].position;
        sim->bodies[i].velocity = solarSystem[i].velocity;
		sim->bodies[i].isAsteroid = 0;

		if (i == 0 || sim->bodies[i].mass > sim->bodies[sim->maxMassBodyIndex].mass)
			sim->maxMassBodyIndex = i;
    }

    for (; i < sim->bodyNumber; i++)
    {
		configureAsteroid(&sim->bodies[i], solarSystem[0].mass);
    }

    return sim;
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    if(!sim)
		return;

    if(sim->bodies)
        free(sim->bodies);

    free(sim);
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    if(!sim)
        return;

    sim->time += sim->timeStep;

    int i;
    for (i = 0; i < sim->bodyNumber; i++)
    {
        Vector3 acceleration = getAcceleration(&sim->bodies[i], sim);
        sim->bodies[i].velocity = getVelocity(sim->bodies[i], acceleration, sim->timeStep);
        sim->bodies[i].position = getPosition(sim->bodies[i], sim->timeStep);
    }

}

static Vector3 getForce(OrbitalBody* body1, OrbitalBody* body2)
{
    if(!body1 || !body2 || body1 == body2)
		return {0, 0, 0};

    Vector3 direction = Vector3Subtract(body2->position, body1->position);

	// Prevent singularity and extreme forces at close distances
    float distance = fmaxf(Vector3Length(direction), MIN_DISTANCE);

	// Only multiply by mass of body2, as body1 is handled in the acceleration calculation
    float magnitude = GRAVITATIONAL_CONSTANT * body2->mass / (distance * distance);

	// Normalize direction to get only the direction and multiply by the magnitude to get the force vector
    if(Vector3Length(direction = Vector3Normalize(direction)) < 0)
        return {0, 0, 0};

    Vector3 force = Vector3Scale(direction, magnitude);
    return force;
}

static Vector3 getAcceleration(OrbitalBody *body, OrbitalSim *sim)
{
    if(!body || !sim || body->mass <= 0)
		return {0, 0, 0};

	// If the body is an asteroid, only consider the most massive body in the system
    if(body->isAsteroid)
    {
		return getForce(body, &sim->bodies[sim->maxMassBodyIndex]);
    }

    Vector3 totalForce = {0, 0, 0};

    int i;
    for(i = 0; i < sim->bodyNumber; i++)
    {
		// Ignore self and very small bodies
        if(&sim->bodies[i] == body || sim->bodies[i].mass <= MASS_THRESHOLD)
			continue;

        Vector3 force = getForce(body, &sim->bodies[i]);
        totalForce = Vector3Add(totalForce, force);
    }

    return totalForce;
}

static Vector3 getVelocity(OrbitalBody body, Vector3 acceleration, float timeStep)
{
    if(timeStep <= 0 || Vector3Length(acceleration) <= 0)
        return body.velocity;

    return Vector3Add(body.velocity, Vector3Scale(acceleration, timeStep));
}

static Vector3 getPosition(OrbitalBody body, float timeStep)
{
    if(timeStep <= 0)
        return body.position;
    
    return Vector3Add(body.position, Vector3Scale(body.velocity, timeStep));
}