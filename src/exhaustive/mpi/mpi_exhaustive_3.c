#include "mpi_exhaustive.h"

/* Global variables */
double *masses = NULL;    // Masses array
Vec3D *positions = NULL;  // Positions array
Vec3D *velocities = NULL; // Velocities array

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

/* Function to update the positions and speeds of bodies */
void run_simulation(FILE *out_fp)
{

    u32 i, j, n, interval;
    double dist_ij, scalar;

    /* set the writing interval to have a maximum of 500 lines in the csv */
    if (num_steps <= 500)
        interval = 1;
    else
        interval = num_steps / 500;

    /*
        each process saves only the speeds of its own bodies in a smaller local vector
    */
    Vec3D localv[my_slice_size];
    for (size_t i = 0; i < my_slice_size; i++)
    {
        localv[i] = velocities[i + my_slice_start];
    }

    for (n = 0; n < num_steps; ++n)
    {
        for (i = my_slice_start; i < my_slice_end; ++i)
        {
            for (j = 0; j < num_bodies; ++j)
            {
                /* calculation of the new velocities of each body */
                if (i == j)
                    continue; // check that i and j aren't the same body 

                dist_ij = vector_distance(&positions[i], &positions[j]);
                scalar = G * masses[j] / (pow(dist_ij, 3.0)) * TIME_DELTA;

                localv[i - my_slice_start].x += scalar * (positions[j].x - positions[i].x);
                localv[i - my_slice_start].y += scalar * (positions[j].y - positions[i].y);
                localv[i - my_slice_start].z += scalar * (positions[j].z - positions[i].z);
            }
        }

        /* calculation and updating of new positions */
        for (i = my_slice_start; i <= my_slice_end; ++i)
        {
            positions[i].x += localv[i - my_slice_start].x * TIME_DELTA;
            positions[i].y += localv[i - my_slice_start].y * TIME_DELTA;
            positions[i].z += localv[i - my_slice_start].z * TIME_DELTA;
        }
        /* communication of new velocities */
        MPI_Allgatherv(positions + my_slice_start, my_slice_size, MPI_Vec3D, positions, count, disp, MPI_Vec3D, MPI_COMM_WORLD);

        /* only the master process writes all updated positions to the file */
        if (n % interval == 0 && my_rank == 0)
        {
            for (size_t i = 0; i < num_bodies; i++)
            {
                fprintf(out_fp, "%lf,%lf,%lf,",
                        positions[i].x,
                        positions[i].y,
                        positions[i].z);
            }

            fprintf(out_fp, "\n");
        }
        //no additional barrier is needed because it is already contained in MPI_Allgheterv
    }
}

int main(int argc, char *argv[])
{
    //double start, finish; UNCOMMENT TO PRINT THE EXECUTION TIME

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
    FILE *in_fp, *out_fp = NULL;


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
    num_bodies = values_readall(in_fp, &masses, &positions, &velocities);

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

    /* Creating Vec3D MPI datatype */
    int AoB[1] = {3};
    MPI_Datatype AoT[1] = {MPI_DOUBLE};
    MPI_Aint AoD[1] = {0};
    MPI_Type_create_struct(1, AoB, AoD, AoT, &MPI_Vec3D);
    MPI_Type_commit(&MPI_Vec3D);

    /* Compute each process' slices of bodies */
    compute_slices();

    /* Run simulation */
    run_simulation(out_fp);
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

    free(positions);
    free(velocities);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
