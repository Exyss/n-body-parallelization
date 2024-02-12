#ifndef READER_H
#define READER_H

/* Library includes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Local includes*/

#include "body.h"

/* Structs */

/* Procedures */

size_t bodies_readall(FILE* input, Body** bodies_ptr);

size_t values_readall(FILE* input, double** masses_ptr, Vec3D** positions_ptr, Vec3D** velocities_ptr);

#endif