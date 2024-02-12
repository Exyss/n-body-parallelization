#ifndef BOUNDING_BOX
#define BOUNDING_BOX

/* Library includes */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Local includes*/
#include "vector.h"

/* Structs */

typedef struct bounding_box {

    /* Cube data */
    double  min_x, mid_x, max_x,
            min_y, mid_y, max_y,
            min_z, mid_z, max_z,
            side;
} BoundingBox;

/* Procedures */

BoundingBox init_box(double min_x, double max_x, double min_y, double max_y, double min_z, double max_z, double side);

BoundingBox init_subbox(BoundingBox box, bool upper, bool north, bool west);

bool is_inside_box(BoundingBox* box, Vec3D* point);

void print_box(BoundingBox* box);

#endif