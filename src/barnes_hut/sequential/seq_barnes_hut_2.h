#ifndef SEQ_BARNES_HUT_2
#define SEQ_BARNES_HUT_2

/* Library includes */

#include <math.h>
#include <errno.h>

/* Local includes*/

#include "../../core/body.h"
#include "../../core/vector.h"
#include "../../core/reader.h"
#include "../../core/bounding_box.h"
#include "bh_octree_2.h"

/* Macros */
#define u32 unsigned int

/* Structs */

/* Simulation data */
const char* OUTPUT_FILE = "output.csv"; // Name of the output file
const double G = 6.674e-11;             // Gravitational constant
const double TIME_DELTA = 1;            // Time difference between each step of the simulation
u32 num_steps;                          // Number of steps of the simulation
u32 num_bodies;                         // Number of bodies in the simulation
double theta;                           // Approximation factor of the simulation

/* Procedures */

void run_simulation(FILE* out_fp);      // Runs the simulation

int main(int argc, char* argv[]);

#endif