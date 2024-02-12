#include "vector.h"
#include <stdio.h>

/*return a Vec3D with the difference from vec1 to vec2*/
Vec3D vector_difference(Vec3D* vec1, Vec3D* vec2){
    Vec3D diff = {
        vec1->x - vec2->x,
        vec1->y - vec2->y,
        vec1->z - vec2->z
    };

    return diff;
}

/*return a Vec3D with the sum from vec1 to vec2*/
Vec3D vector_addition(Vec3D* vec1, Vec3D* vec2){
    Vec3D diff = {
        vec1->x + vec2->x,
        vec1->y + vec2->y,
        vec1->z + vec2->z
    };

    return diff;
}

/*sum vec2 to vec1*/
void sum_vector(Vec3D* vec1, Vec3D* vec2){
    vec1->x += vec2->x;
    vec1->y += vec2->y;
    vec1->z += vec2->z;
}

/*subtract vec2 to vec1*/
void sub_vector(Vec3D* vec1, Vec3D* vec2){
    vec1->x -= vec2->x;
    vec1->y -= vec2->y;
    vec1->z -= vec2->z;
}

/* returns a scaled vector of the given value*/
Vec3D scale_vector(Vec3D* vec, double scalar){
    Vec3D scaled = {
        vec->x * scalar,
        vec->y * scalar,
        vec->z * scalar
    };

    return scaled;
}

/* returns the norm of the vector*/
double vector_norm(Vec3D* vec){
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

/*return the distance between two vectors*/
double vector_distance(Vec3D* vec1, Vec3D* vec2) {
    Vec3D vec_diff = vector_difference(vec2, vec1);
    return vector_norm(&vec_diff);
}


/*print the vector */
void print_vector(Vec3D* vec){
    printf("(%g, %g, %g)\n", vec->x, vec->y, vec->z);
}