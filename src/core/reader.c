#include "reader.h"

/* Reads bodies from the given file, returning the number of read bodies */

size_t bodies_readall(FILE* input, Body** bodies_ptr){
    Body* bodies= *bodies_ptr;
    size_t i = 0;
    size_t num_values = 0;

    /* If at least one of the parameters is NULL. */
    if (!input || !bodies_ptr) {
        errno = EINVAL;
        return 0;
    }

    /* If input stream is already in error state. */
    if (ferror(input)) {
        errno = EIO;
        return 0;
    }

    while (1) {
        bodies = realloc(bodies, sizeof(*bodies) * (i+1));      //TODO: could probably be implemented better

        /* If realloc failed */
        if (!bodies) {
            errno = ENOMEM;
            return 0;
        }

        *bodies_ptr = bodies;
        
        num_values = fscanf(input, " %lf, %lf, %lf, %lf, %lf, %lf, %lf;",
                        &bodies[i].mass,
                        &bodies[i].pos.x, &bodies[i].pos.y, &bodies[i].pos.z,
                        &bodies[i].vel.x, &bodies[i].vel.y, &bodies[i].vel.z
                    );

        /* If less than 7 valid values were read*/
        if (num_values != 7) break;

        i++;
    }

    /*  If there was an actual I/O error or the file contains unread data,
        set errno to EIO, otherwise set it to 0 */
    errno = (ferror(input) || !feof(input)) ? EIO : 0;

    return i;
}

/* Reads bodies data from the given file without using Body struct, returning the number of read bodies */

size_t values_readall(FILE* input, double** masses_ptr, Vec3D** positions_ptr, Vec3D** velocities_ptr){
    
    double* masses = *masses_ptr;
    Vec3D* positions = *positions_ptr;
    Vec3D* velocities = *velocities_ptr;
    
    size_t i = 0;
    size_t num_values = 0;

    /* If at least one of the parameters is NULL. */
    if (!input || !masses_ptr || !velocities_ptr || !positions_ptr) {
        errno = EINVAL;
        return 0;
    }

    /* If input stream is already in error state. */
    if (ferror(input)) {
        errno = EIO;
        return 0;
    }

    while (1) {
        masses = realloc(masses, sizeof(*masses) * (i+1));
        positions = realloc(positions, sizeof(*positions) * (i+1));
        velocities = realloc(velocities, sizeof(*velocities) * (i+1));

        /* If realloc failed */
        if (!masses || !velocities || !positions) {
            errno = ENOMEM;
            return 0;
        }

        *masses_ptr = masses;
        *positions_ptr = positions;
        *velocities_ptr = velocities;
        
        num_values = fscanf(input, " %lf, %lf, %lf, %lf, %lf, %lf, %lf;",
                        &masses[i],
                        &positions[i].x, &positions[i].y, &positions[i].z,
                        &velocities[i].x, &velocities[i].y, &velocities[i].z
                    );

        /* If less than 7 valid values were read*/
        if (num_values != 7) break;

        i++;
    }

    /*  If there was an actual I/O error or the file contains unread data,
        set errno to EIO, otherwise set it to 0 */
    errno = (ferror(input) || !feof(input)) ? EIO : 0;

    return i;
}
