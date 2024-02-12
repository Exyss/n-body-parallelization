#include "mpi_barnes_hut.h"

/* Global variables */
Body *bodies = NULL;      // Bodies array

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

/* Runs the simulation using the set simulation data and the given file pointer as output file */
void run_simulation(FILE* out_fp)
{
    u32 i, n, interval;
    BHOctree *bho;
    BHOctree **leaves = NULL;
    Vec3D sum_vel;
    
    /*
        each process saves only the speeds of its own bodies in a smaller local vector
    */
    Vec3D localv[my_slice_size];
    for (size_t i = 0; i < my_slice_size; i++)
    {
        localv[i] = bodies[i + my_slice_start].vel;
    }

    /* set the writing interval to have a maximum of 500 lines in the csv */
    if (num_steps <= 500)
        interval = 1;
    else
        interval = num_steps / 500;

    leaves = malloc(sizeof(BHOctree*) * num_bodies);
    bho = generate_bodies_bho(bodies, leaves, num_bodies);

    for (n = 0; n < num_steps; ++n)
    {
        for (i = my_slice_start; i <= my_slice_end; ++i)
        {
            /* calculation of the new velocities of each body */
            sum_vel = compute_velocity_increment(bho, &bodies[i], G, theta);
            localv[i - my_slice_start].x += sum_vel.x * TIME_DELTA;
            localv[i - my_slice_start].y += sum_vel.y * TIME_DELTA;
            localv[i - my_slice_start].z += sum_vel.z * TIME_DELTA;
        }

        /* calculation and updating of new positions */
        for (i = my_slice_start; i <= my_slice_end; ++i)
        {
            bodies[i].pos.x += localv[i - my_slice_start].x * TIME_DELTA;
            bodies[i].pos.y += localv[i - my_slice_start].y * TIME_DELTA;
            bodies[i].pos.z += localv[i - my_slice_start].z * TIME_DELTA;
        }

        /* communication of new velocities */
        MPI_Allgatherv(bodies+my_slice_start, my_slice_size, MPI_Body, bodies, count, disp, MPI_Body, MPI_COMM_WORLD);

        update_bodies_bho(bho,bodies,leaves, num_bodies);        

        /* only the master process writes all updated positions to the file */
        if (n % interval == 0 && my_rank == 0)
        {
            for (size_t i = 0; i < num_bodies; i++)
            {
                fprintf(out_fp, "%lf,%lf,%lf,",
                        bodies[i].pos.x,
                        bodies[i].pos.y,
                        bodies[i].pos.z);
            }

            fprintf(out_fp, "\n");
        }
        //no additional barrier is needed because it is already contained in MPI_Allgheterv
    }
    free(leaves);
    destroy_bhoctree(bho);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Correct usage: %s <input_file> <simulation_steps> <approx_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    //start = MPI_Wtime(); UNCOMMENT TO PRINT THE EXECUTION TIME
    const char *input_file = argv[1];
    FILE *in_fp;
    FILE *out_fp = NULL;

    theta = strtod(argv[3], NULL);
    num_steps = strtol(argv[2], NULL, 10);

    in_fp = fopen(input_file, "r");

    /* If file opening failed*/
    if (!in_fp)
    {
        fprintf(stderr, "Cannot open %s: %s.\n", input_file, strerror(errno));
        return EXIT_FAILURE;
    }

    /* Read in_fp file */
    num_bodies = bodies_readall(in_fp, &bodies);

    /* Properly close file */
    if (fclose(in_fp))
    {
        fprintf(stderr, "Error closing %s.\n", input_file);
        return EXIT_FAILURE;
    }

    /* If data parsing failed */
    if (errno)
    {
        fprintf(stderr, "Parsing error: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (my_rank == 0)
    {
    out_fp = fopen(OUTPUT_FILE, "w");

    if (!out_fp)
    {
        fprintf(stderr, "Cannot open %s: %s.\n", OUTPUT_FILE, strerror(errno));
        return EXIT_FAILURE;
    }
    }
    // creating MPI_Vec3D
    MPI_Datatype MPI_Vec3D;
    int AoB[1] = {3};
    MPI_Datatype AoT[1] = {MPI_DOUBLE};
    MPI_Aint AoD[1] = {0};
    MPI_Type_create_struct(1, AoB, AoD, AoT, &MPI_Vec3D);
    MPI_Type_commit(&MPI_Vec3D);

    // creating MPI_body
    int AoB2[2] = {1, 2};
    MPI_Datatype AoT2[2] = {MPI_DOUBLE, MPI_Vec3D};
    MPI_Aint AoD2[2] = {0};
    MPI_Aint a_addr, b_addr;

    MPI_Get_address(&bodies[0].mass, &a_addr);
    MPI_Get_address(&bodies[0].pos.x, &b_addr);

    AoD2[1] = b_addr - a_addr;
    MPI_Type_create_struct(2, AoB2, AoD2, AoT2, &MPI_Body);
    MPI_Type_commit(&MPI_Body);

    compute_slices();

    /* Run simulation */
    run_simulation(out_fp);

    //finish = MPI_Wtime(); UNCOMMENT TO PRINT THE EXECUTION TIME
    //printf("|%d: %g| ", my_rank, finish - start); UNCOMMENT TO PRINT THE EXECUTION TIME

    if (my_rank == 0)
    {
        if (fclose(out_fp))
        {
            fprintf(stderr, "Error closing %s.\n", OUTPUT_FILE);
            return EXIT_FAILURE;
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
