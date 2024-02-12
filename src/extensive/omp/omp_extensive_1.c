/* Imports */
#include "omp_extensive.h"

/* Global variables */
Body* bodies;

/* Runs the simulation using the set simulation data and the given file pointer as output file */
void run_simulation(FILE* out_fp) {
    u32 i, j, k, interval;
    double dist_ij, scalar;

    /* set the writing interval to have a maximum of 500 lines in the csv */
    if(num_steps <= 500)
        interval = 1;
    else
        interval = num_steps / 500;
    
    #pragma omp parallel num_threads(thread_count) \
        default(none) shared(bodies, num_bodies, num_steps, out_fp, interval) \
        private(i, j, k, dist_ij, scalar)
    {
        for (k = 0; k < num_steps; ++k){

            /* calculation of the new velocities of each body */
            #pragma omp for
            for (i = 0; i < num_bodies; ++i) {
                for (j = 0; j < num_bodies; ++j) {
                    if(i == j) continue; // check that i and j aren't the same body 

                    dist_ij = vector_distance(&bodies[i].pos, &bodies[j].pos);
                    scalar = G * bodies[j].mass * (pow(dist_ij, -3.0)) * TIME_DELTA;

                    bodies[i].vel.x += scalar * (bodies[j].pos.x - bodies[i].pos.x);
                    bodies[i].vel.y += scalar * (bodies[j].pos.y - bodies[i].pos.y);
                    bodies[i].vel.z += scalar * (bodies[j].pos.z - bodies[i].pos.z);
                }
            }
            
            // Implicit barrier here thanks to #pragma omp for 

            /* calculation and updating of new positions */
            #pragma omp for
            for (i = 0; i < num_bodies; ++i) {
                bodies[i].pos.x += bodies[i].vel.x * TIME_DELTA;
                bodies[i].pos.y += bodies[i].vel.y * TIME_DELTA;
                bodies[i].pos.z += bodies[i].vel.z * TIME_DELTA;
            }
            
            // Implicit barrier here thanks to #pragma omp for 

            // Only the master thread writes on file
            if(k % interval == 0){
                #pragma omp single
                {
                    for (size_t i = 0; i < num_bodies; i++) {
                        fprintf(out_fp,"%lf,%lf,%lf,", bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z);
                    }

                    fprintf(out_fp,"\n");
                }
            }
        }    
    }
}

int main(int argc, char* argv[]){
    const char* input_file;
    FILE *in_fp, *out_fp;

    /* Print correct usage */
    if (argc < 4){
        fprintf(stderr, "Correct usage: %s <input_file> <simulation_steps> <thread_count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Get input values */
    input_file = argv[1];
    num_steps = strtol(argv[2], NULL, 10);
    thread_count = strtol(argv[3], NULL, 10);
    
    /* Read input data */
    in_fp = fopen(input_file, "r");

    if (!in_fp) {
        fprintf(stderr, "Cannot open %s: %s.\n", input_file, strerror(errno));
        return EXIT_FAILURE;
    }

    num_bodies = bodies_readall(in_fp, &bodies);
    
    if (fclose(in_fp)) {
        fprintf(stderr, "Error closing %s.\n", input_file);
        return EXIT_FAILURE;
    }

    if (errno) {
        fprintf(stderr, "Parsing error: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* Open output file */
    out_fp = fopen(OUTPUT_FILE, "w");

    if (!out_fp) {
        fprintf(stderr, "Cannot open %s: %s.\n", OUTPUT_FILE, strerror(errno));
        return EXIT_FAILURE;
    }

    /* Run simulation */
    run_simulation(out_fp);

    if (fclose(out_fp)) {
        fprintf(stderr, "Error closing %s.\n", OUTPUT_FILE);
        return EXIT_FAILURE;
    }
    
    free(bodies);

    return EXIT_SUCCESS;
}
