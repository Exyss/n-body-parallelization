#ifndef MPI_barnes_hut
#define MPI_barnes_hut

/* Library includes */

#include <math.h>
#include <errno.h>
#include <mpi.h>

/* Local includes*/

#include "../../core/body.h"
#include "../../core/vector.h"
#include "../../core/reader.h"
#include "../../core/bounding_box.h"
#include "bh_octree.h"

/* Macros */
#define u32 unsigned int

/* Structs */

/* Simulation data */
const char* OUTPUT_FILE = "output.csv"; // Name of the output file
const double G = 6.674e-11;     // Gravitational constant
const double TIME_DELTA = 1;    // Time difference between each step of the simulation
double theta;             // Approximation threshold
u32 num_steps;                          // Number of steps of the simulation
u32 num_bodies;                         // Number of bodies in the simulation     


/* MPI data */
int *disp, *count;
MPI_Datatype MPI_Body;
int my_rank, comm_size;
u32 my_slice_start, my_slice_size, my_slice_end;
//double start, finish;

/* Procedures */

void run_simulation(FILE* out_fp);

#endif