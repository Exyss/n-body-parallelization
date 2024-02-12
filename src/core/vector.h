#ifndef VECTOR_H
#define VECTOR_H

/* Library includes */

#include <math.h>

/* Local includes*/

/* Structs */

typedef struct vector3D{
    double x;
    double y;
    double z;
} Vec3D;

/* Procedures */

Vec3D vector_difference(Vec3D* vec1, Vec3D* vec2);

Vec3D vector_addition(Vec3D* vec1, Vec3D* vec2);

void sum_vector(Vec3D* vec1, Vec3D* vec2);

void sub_vector(Vec3D* vec1, Vec3D* vec2);

Vec3D scale_vector(Vec3D* vec, double scalar);

double vector_norm(Vec3D* vec);

double vector_distance(Vec3D* vec1, Vec3D* vec2);

void print_vector(Vec3D* vec);

#endif