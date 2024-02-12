#ifndef BODY_H
#define BODY_H

/* Library includes */

/* Local includes*/

#include "vector.h"

/* Structs */

typedef struct body {
    double mass;
    Vec3D pos;
    Vec3D vel;
} Body;

/* Procedures */

void print_body(Body* body);

#endif