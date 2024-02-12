#include "seq_barnes_hut_2.h"

/* Global variables */
Body* bodies;

/* Runs the simulation using the set simulation data and the given file pointer as output file */
void run_simulation(FILE* out_fp) {
    u32 i, k, interval;
    BHOctree* bho; 
    BHOctree** leaves;
    Vec3D sum_vel;

    if(num_steps <= 500)
        interval = 1;
    else
        interval = num_steps / 500;

    leaves = malloc(sizeof(BHOctree*) * num_bodies);
    bho = generate_bodies_bho(bodies, leaves, num_bodies);

    for (k = 0; k < num_steps; ++k){

        /* calculation of the new velocities of each body */
        for (i = 0; i < num_bodies; ++i) {
            sum_vel = compute_velocity_increment(bho, &bodies[i], G, theta);
            bodies[i].vel.x += sum_vel.x * TIME_DELTA;
            bodies[i].vel.y += sum_vel.y * TIME_DELTA;
            bodies[i].vel.z += sum_vel.z * TIME_DELTA;
        }

        /* calculation and updating of new positions */
        for (i = 0; i < num_bodies; ++i) {
            bodies[i].pos.x += bodies[i].vel.x * TIME_DELTA;
            bodies[i].pos.y += bodies[i].vel.y * TIME_DELTA;
            bodies[i].pos.z += bodies[i].vel.z * TIME_DELTA;
        }

        /* the tree is updated by moving nodes between branches where necessary */
        bho = update_bodies_bho(bho, bodies, leaves, num_bodies); 

        /* writing the result of the iteration to the csv file */
        if (k % interval == 0) {
            for (size_t i = 0; i < num_bodies; i++){
                fprintf(out_fp, "%lf,%lf,%lf,", bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z);
            }

            fprintf(out_fp, "\n");
        }
    }
    /*the tree is destroyed only at the end of the simulation*/
    destroy_bhoctree(bho);
}

int main(int argc, char* argv[]){
    const char* input_file;
    FILE *in_fp, *out_fp;

    /* Print correct usage */
    if (argc < 4){
        fprintf(stderr, "Correct usage: %s <input_file> <simulation_steps> <approx_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Get input values */
    input_file = argv[1];
    num_steps = strtol(argv[2], NULL, 10);
    theta = strtod(argv[3], NULL);

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
