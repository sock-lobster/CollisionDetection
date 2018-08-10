#ifndef CONSTANTS_H
#define CONSTANTS_H

/** @file constants.h
 *
 *
 */

// MODIFIABLE BY USER
/// The number of particles.
extern int NUM_PARTICLES;
extern double MIN_PARTICLE_RADIUS;
extern double MAX_PARTICLE_RADIUS;
extern double MIN_VELOCITY;
extern double MAX_VELOCITY;
extern double X_DIMENSION_SIZE;
extern double Y_DIMENSION_SIZE;
extern double Z_DIMENSION_SIZE;
/// The seed used by the random number generator.
extern int SEED;
extern double LOOKAHEAD_TIME;   // in seconds;
extern double MAX_LOOKAHEAD_TIME;
extern int MAX_QUEUE_SIZE;
extern int TIMESTEP_LIMIT;
extern int FRAMERATE;
extern int TARGET_FRAMERATE;

extern int LOOKAHEAD_TIMESTEPS;

// NOT MODIFIABLE BY USER
const int MIN_FRAMERATE = 15;
const int MAX_FRAMERATE = 510;
const int QUEUE_SIZE_MULTIPLIER = 1000;
// 3D
const int ALG_SPEED_SCALE = 50;
const double DISPLAY_SIDE = 600;
const double SIZE_MULTIPLIER_2D = DISPLAY_SIDE / X_DIMENSION_SIZE * 0.5;
const double VELOCITY_MULTIPLIER = 0.0003 * X_DIMENSION_SIZE / ALG_SPEED_SCALE;
const int MIN_NUM_PARTICLES = 1;
const int MAX_NUM_PARTICLES = 20000;
extern double GRAVITY;

// SWEEP AND PRUNE
const int X_DIMENSION = 0;
const int Y_DIMENSION = 1;
const int Z_DIMENSION = 2;

// QUADTREE
extern int MAX_PARTICLES_PER_LEVEL;
extern int MAX_LEVELS;

#endif // CONSTANTS_H
