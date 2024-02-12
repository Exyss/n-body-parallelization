#ifndef exhaustive_MPI
#define exhaustive_MPI

/* Library includes */
#include <mpi.h>
#include <math.h>
#include <errno.h>

/* Local includes*/
#include "../../core/body.h"
#include "../../core/vector.h"
#include "../../core/reader.h"

/* Macros */
#define u32 unsigned int

/* Structs */

/* Simulation data */
const double G = 6.674e-11;             // Gravitational constant
const double TIME_DELTA = 1;            // Time difference between each step of the simulation
u32 num_steps;                          // Number of steps of the simulation
u32 num_bodies;                         // Number of bodies in the simulation        
const char* OUTPUT_FILE = "output.csv"; // Name of the output file  

/* MPI data */
int *disp, *count;
int my_rank, comm_size;
u32 my_slice_size, my_slice_start, my_slice_end;
MPI_Datatype MPI_Vec3D, MPI_Body;

/* Procedures */

void create_MPI_datatypes();            // Creates MPI_Vec3D and MPI_Body

void compute_slices();                  // Subdivide bodies array among proccesses

void run_simulation(FILE* out_fp);      // Runs the simulation

int main(int argc, char* argv[]);

#endif