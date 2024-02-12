#ifndef OMP_extensive
#define OMP_extensive

/* Library includes */

#include <math.h>
#include <errno.h>
#include <omp.h>

/* Local includes*/

#include "../../core/body.h"
#include "../../core/vector.h"
#include "../../core/reader.h"

/* Macros */
#define u32 unsigned int

/* Structs */

/* Simulation data */
const char* OUTPUT_FILE = "output.csv"; // Name of the output file
const double G = 6.674e-11;             // Gravitational constant
const double TIME_DELTA = 1;            // Time difference between each step of the simulation
u32 num_steps;                          // Number of steps of the simulation
u32 num_bodies;                         // Number of bodies in the simulation          
u32 thread_count;                       // Number of threads to execute the simulation

/* Procedures */

void run_simulation(FILE* out_fp);      // Runs the simulation

int main(int argc, char* argv[]);

#endif