#ifndef BH_OCTREE_2
#define BH_OCTREE_2

/* Library includes */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Local includes*/

#include "../../core/bounding_box.h"
#include "../../core/body.h"

/* Macros */
#define u8 unsigned char
#define u32 unsigned int

#define EMPTY 0
#define SINGLE_BODY 1
#define SUBDIVIDED 2

/* Structs */

typedef struct barnes_hut_octree {

    /* != NULL only if status = 1*/
    Body* body;

    /* Space section limits */
    BoundingBox box;
    
    // 0 = Empty, 1 = Single body, 2 = Subdivided
    u8 status;

    /* Pseudo-body data */
    double total_mass;
    Vec3D center_of_mass;

    /*
        Each index represents a sub-section:

        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
        -------------------------------------------------
        | UNW | UNE | USW | USE | LNW | LNE | LSW | LSE |
    */
    struct barnes_hut_octree* sub_sections[8];

    /* Optimized-update data*/
    struct barnes_hut_octree** leaf;
    struct barnes_hut_octree* parent;
    unsigned int parent_sub_index;
} BHOctree;

/* Procedures */

BHOctree* init_bhoctree(BHOctree* parent, unsigned int parent_sub_index, BoundingBox box);

BHOctree* generate_bodies_bho(Body* bodies, BHOctree** leaves, u32 num_bodies);

void insert_body(BHOctree* bho, Body* body, BHOctree** leaf);

BHOctree* update_bodies_bho(BHOctree* bho, Body* bodies, BHOctree** leaves, u32 num_bodies);

void compute_pseudo_bodies(BHOctree* bho);

Vec3D compute_velocity_increment(BHOctree* bho, Body* body, const double G, const double theta);

void destroy_bhoctree(BHOctree* bho);

void _print_bhoctree(BHOctree *bho, size_t deep);

#endif