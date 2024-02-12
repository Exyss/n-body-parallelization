#include <math.h>
#include "body.h"
#include <stdio.h>

/* print the body */
void print_body(Body* body){
    printf("mass: %g, position: (%g, %g, %g), velocity: (%g, %g, %g)\n",
        body->mass, body->pos.x, body->pos.y, body->pos.z,
        body->vel.x, body->vel.y, body->vel.z);
}
