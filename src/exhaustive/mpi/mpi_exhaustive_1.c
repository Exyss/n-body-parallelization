/* Imports */
#include "mpi_exhaustive.h"


/* Global variables */
Body *bodies;      // Bodies array

/*
    Returns the interval between each simulation output.
    The interval is computed in order to always obtain at most 500 output lines.
*/
void create_MPI_datatypes(){
    /*
        AoB = Array of Blocks
        AoT = Array of Types
        AoD = Array of Displacements
    */
    Body dummy = {0, {0, 0, 0}, {0, 0, 0}};

    MPI_Aint a_addr, b_addr;

    // Vec3D contains 3 doubles
    int AoB_Vec3D[1] = {3};
    MPI_Datatype AoT_Vec3D[1] = {MPI_DOUBLE};
    MPI_Aint AoD_Vec3D[1] = {0};

    MPI_Type_create_struct(1, AoB_Vec3D, AoD_Vec3D, AoT_Vec3D, &MPI_Vec3D);
    MPI_Type_commit(&MPI_Vec3D);

    // Body contains 1 double and 2 Vec3D
    int AoB_Body[2] = {1, 2};
    MPI_Datatype AoT_Body[2] = {MPI_DOUBLE, MPI_Vec3D};

    // Find displacement from double to Vec3D
    MPI_Get_address(&dummy.mass, &a_addr);
    MPI_Get_address(&dummy.pos.x, &b_addr);
    MPI_Aint AoD_Body[2] = {0, b_addr - a_addr};

    MPI_Type_create_struct(2, AoB_Body, AoD_Body, AoT_Body, &MPI_Body);
    MPI_Type_commit(&MPI_Body);
}

/*  
    Function to calculate displacement and number of elements for each process 
    based on the total number of elements and the process rank
*/
void compute_slices()
{
    int remainder;

    my_slice_size = num_bodies / comm_size;
    my_slice_start = my_rank * my_slice_size;
    remainder = num_bodies % comm_size;

    if (my_rank < remainder) // it then takes one of the remaining bodies
    {
        my_slice_size++;
        my_slice_start += my_rank; // and sets appropriately its displacement
    }
    else
        my_slice_start += remainder; // just set the movement

    my_slice_end = my_slice_start + my_slice_size - 1;

    /* creating the vectors for body count and displacement of each process */
    count = (int *)malloc(comm_size * sizeof(int));
    disp = (int *)malloc(comm_size * sizeof(int));

    count[my_rank] = my_slice_size;
    disp[my_rank] = my_slice_start;

    MPI_Allgather(&my_slice_size, 1, MPI_INT, count, 1, MPI_INT, MPI_COMM_WORLD); //each process shares its body count and get the body count of the other processes
    MPI_Allgather(&my_slice_start, 1, MPI_INT, disp, 1, MPI_INT, MPI_COMM_WORLD); //each process shares its displacement and get the displacement of the other processes
}

// Function to update the positions and speeds of bodies
void run_simulation(FILE* out_fp) {
    u32 i, j, n, interval;
    double dist_ij, scalar;

    /* set the writing interval to have a maximum of 500 lines in the csv */
    if (num_steps <= 500)
        interval = 1;
    else
        interval = num_steps / 500;

    for (n = 0; n < num_steps; ++n){
        for (i = my_slice_start; i <= my_slice_end; ++i){
            for (j = 0; j < num_bodies; ++j){
                /* calculation of the new velocities of each body */
                if (i == j) continue; // check that i and j aren't the same body 

                dist_ij = vector_distance(&bodies[i].pos, &bodies[j].pos);
                scalar = G * bodies[j].mass * (pow(dist_ij, -3.0)) * TIME_DELTA;

                bodies[i].vel.x += scalar * (bodies[j].pos.x - bodies[i].pos.x);
                bodies[i].vel.y += scalar * (bodies[j].pos.y - bodies[i].pos.y);
                bodies[i].vel.z += scalar * (bodies[j].pos.z - bodies[i].pos.z);
            }
        }

        /* calculation and updating of new positions */
        for (i = my_slice_start; i <= my_slice_end; ++i)
        {
            bodies[i].pos.x += bodies[i].vel.x * TIME_DELTA;
            bodies[i].pos.y += bodies[i].vel.y * TIME_DELTA;
            bodies[i].pos.z += bodies[i].vel.z * TIME_DELTA;
        }

        /* communication and new velocities */
        MPI_Allgatherv(bodies + my_slice_start, my_slice_size, MPI_Body,
            bodies, count, disp, MPI_Body, MPI_COMM_WORLD);

        /* only the master process writes all updated positions to the file */
        if (n % interval == 0 && my_rank == 0){
            for (size_t i = 0; i < num_bodies; i++){
                fprintf(out_fp, "%lf,%lf,%lf,", bodies[i].pos.x, bodies[i].pos.y, bodies[i].pos.z);
            }

            fprintf(out_fp, "\n");
        }
        //no additional barrier is needed because it is already contained in MPI_Allgheterv
    }
}

int main(int argc, char *argv[])
{
    /* Get MPI data */
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    /* Start timer */
    //start = MPI_Wtime(); UNCOMMENT TO PRINT THE EXECUTION TIME

    /* Print correct usage */
    if (argc < 3)
    {
        fprintf(stderr, "Correct usage: %s <input_file> <simulation_steps>\n", argv[0]);
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    
    const char *input_file;
    FILE *in_fp, *out_fp;

    /* Get input values */
    input_file = argv[1];
    num_steps = strtol(argv[2], NULL, 10);

    /* Read input data */
    in_fp = fopen(input_file, "r");

    /* If file opening failed*/
    if (!in_fp)
    {
        fprintf(stderr, "Cannot open %s: %s.\n", input_file, strerror(errno));
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* Read in_fp file */
    num_bodies = bodies_readall(in_fp, &bodies);

    /* Properly close file */
    if (fclose(in_fp))
    {
        fprintf(stderr, "Error closing %s.\n", input_file);
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* If data parsing failed */
    if (errno)
    {
        fprintf(stderr, "Parsing error: %s.\n", strerror(errno));
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* Open output file - Only the master thread writes the output */
    if (my_rank == 0)
    {
        out_fp = fopen(OUTPUT_FILE, "w");

        if (!out_fp)
        {
            fprintf(stderr, "Cannot open %s: %s.\n", OUTPUT_FILE, strerror(errno));
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else
        out_fp = NULL;

    /* Create body MPI datatype */
    create_MPI_datatypes();

    /* Compute each process' slices of bodies */
    compute_slices();

    /* Run simulation */
    run_simulation(out_fp);
    free(bodies);

    //finish = MPI_Wtime(); UNCOMMENT TO PRINT THE EXECUTION TIME

    //printf("%g,", finish - start); UNCOMMENT TO PRINT THE EXECUTION TIME

    if (my_rank == 0)
    {
        if (fclose(out_fp))
        {
            fprintf(stderr, "Error closing %s.\n", OUTPUT_FILE);
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
