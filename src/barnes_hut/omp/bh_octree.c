#include "bh_octree.h"

/* Function to create a new empty Barnes Hut octotree*/
BHOctree* init_bhoctree(BHOctree* parent, unsigned int parent_sub_index, BoundingBox box){
    BHOctree* bho = calloc(1, sizeof(BHOctree));

    bho->body = NULL;
    bho->box = box;
    bho->status = EMPTY;

    bho->leaf = NULL;
    bho->parent = parent;
    bho->parent_sub_index = parent_sub_index;

    for(int i = 0; i < 8; i++) bho->sub_sections[i] = NULL;

    return bho;
}

/* Create a Barnes Hut octotree from the bodies and leaves nodes */
BHOctree* generate_bodies_bho(Body* bodies, BHOctree** leaves, u32 num_bodies){
    double init_half_side = 0;
    BoundingBox box;
    BHOctree* bho;

    /* Find maximum distance from origin in every coordinate */
    for(size_t i = 0; i < num_bodies; ++i){
        if(fabs(bodies[i].pos.x) > init_half_side)    init_half_side = fabs(bodies[i].pos.x);
        if(fabs(bodies[i].pos.y) > init_half_side)    init_half_side = fabs(bodies[i].pos.y);
        if(fabs(bodies[i].pos.z) > init_half_side)    init_half_side = fabs(bodies[i].pos.z);
    }

    init_half_side *= 1.5;

    /* Create initial cube */
    box = init_box(-init_half_side, init_half_side, -init_half_side, init_half_side, -init_half_side, init_half_side, 2*init_half_side);
    bho = init_bhoctree(NULL, 0, box);

    for(size_t i = 0; i < num_bodies; ++i){
        insert_body(bho, &bodies[i], &leaves[i]);
    }

    compute_pseudo_bodies(bho);

    return bho;
}

/* insert a node in a BHOctree*/
void insert_body(BHOctree* bho, Body* body, BHOctree** leaf){
    bool U, N, W;
    unsigned int section;
    Body* tmp;
    BHOctree** tmp_leaf;
    BHOctree* sub_bho;
    BoundingBox box;

    switch(bho->status){
        case EMPTY:
            bho->status = SINGLE_BODY;
            bho->body = body;
            
            // This node stores his leaf for future re-insert
            bho->leaf = leaf;

            // Make the leaf point to this node
            *leaf = bho;

            break;

        case SINGLE_BODY:
            // Re-insert bodies treating the section as subdivided
            bho->status = SUBDIVIDED;

            tmp = bho->body;
            tmp_leaf = bho->leaf;

            bho->body = NULL;
            bho->leaf = NULL;

            insert_body(bho, tmp, tmp_leaf);
            insert_body(bho, body, leaf);

            break;

        default:
            /* Compute sub-section in which the body will be inserted */
            box = bho->box;
            U = (body->pos.y < box.mid_y) ? 1 : 0;  // Upper/Down section 
            N = (body->pos.z < box.mid_z) ? 1 : 0;  // North/South section 
            W = (body->pos.x < box.mid_x) ? 1 : 0;  // West/East section 

            section = U * 4 + N * 2 + W * 1;

            if(bho->sub_sections[section] != NULL){
                insert_body(bho->sub_sections[section], body, leaf);
            }
            else{
                sub_bho = init_bhoctree(bho, section, init_subbox(box, U, N, W));
                bho->sub_sections[section] = sub_bho;
                insert_body(sub_bho, body, leaf);
            }

            break;
    }
}

/*  Updates the location of each body in the tree. If needed, the tree gets completely reconstructed */
BHOctree* update_bodies_bho(BHOctree* bho, Body* bodies, BHOctree** leaves, u32 num_bodies){
    unsigned int i, leaf_index;
    
    for(i = 0; i < num_bodies; ++i){
        if(!is_inside_box(&leaves[i]->box, &bodies[i].pos)){
                        
            // If the body went outside of the radix box, regenerate the whole tree
            if(!is_inside_box(&bho->box, &bodies[i].pos)){
                destroy_bhoctree(bho);
                bho = generate_bodies_bho(bodies, leaves, num_bodies);
                break;
            }

            // Else, move the body on a different octant
            BHOctree* parent = leaves[i]->parent;
            leaf_index = leaves[i]->parent_sub_index;

            if(parent->sub_sections[leaf_index] == leaves[i])
                parent->sub_sections[leaf_index]->status = EMPTY;

            while(!is_inside_box(&parent->box, &bodies[i].pos)){
                parent = parent->parent;
            }

            insert_body(parent, &bodies[i], &leaves[i]);
        }
    }

    compute_pseudo_bodies(bho);
    return bho;
}

/*calculate the center of mass and total mass of each pseudo bodies of the tree*/
void compute_pseudo_bodies(BHOctree* bho){

    double total_mass;
    Vec3D center_of_mass;
    BHOctree* sub_bho;

    if(!bho)
        return;
    
    switch(bho->status){
        case EMPTY:
            bho->total_mass = 0;
            bho->center_of_mass.x = 0;
            bho->center_of_mass.y = 0;
            bho->center_of_mass.z = 0;

            break;

        case SINGLE_BODY:
            bho->total_mass = bho->body->mass;
            bho->center_of_mass = bho->body->pos;

            break;

        default:
            total_mass = 0;
            center_of_mass.x = 0;
            center_of_mass.y = 0;
            center_of_mass.z = 0;

            for (int i = 0; i < 8; ++i){
                sub_bho = bho->sub_sections[i];

                if(sub_bho){    // if != NULL
                    compute_pseudo_bodies(sub_bho);

                    total_mass += sub_bho->total_mass;
                    center_of_mass.x += sub_bho->center_of_mass.x * sub_bho->total_mass;
                    center_of_mass.y += sub_bho->center_of_mass.y * sub_bho->total_mass;
                    center_of_mass.z += sub_bho->center_of_mass.z * sub_bho->total_mass;
                }
            }

            bho->total_mass = total_mass;

            if(total_mass == 0){
                bho->center_of_mass.x = 0;
                bho->center_of_mass.y = 0;
                bho->center_of_mass.z = 0;
            }
            else{
                bho->center_of_mass.x = center_of_mass.x / total_mass;
                bho->center_of_mass.y = center_of_mass.y / total_mass;
                bho->center_of_mass.z = center_of_mass.z / total_mass;
            }

            break;
    }    
}

/* calculates the speedup for the input body under the influence of the bodies in bho */
Vec3D compute_velocity_increment(BHOctree* bho, Body* body, const double G, const double theta){
    double dist, scalar, ratio;
    Vec3D new_vel, sub_vel;

    new_vel.x = 0;
    new_vel.y = 0;
    new_vel.z = 0;

    if(!bho || bho->status == EMPTY || (bho->status == SINGLE_BODY && bho->body == body))
        return new_vel;

    /* If the node contains a single body, that body is the center of mass */
    dist = vector_distance(&body->pos, &bho->center_of_mass);

    if(bho->status != SINGLE_BODY ){
        ratio = bho->box.side / dist;

        /* Do not approximate */
        if(ratio >= theta){
            for(int i = 0; i < 8; ++i){
                if(!bho->sub_sections[i] || bho->sub_sections[i]->status == EMPTY)
                    continue;

                sub_vel = compute_velocity_increment(bho->sub_sections[i], body, G, theta);

                new_vel.x += sub_vel.x;
                new_vel.y += sub_vel.y;
                new_vel.z += sub_vel.z;
            }

            return new_vel;
        }
    }

    /* Approximate */

    scalar = G * bho->total_mass * (pow(dist, -3.0));
    new_vel.x += scalar * (bho->center_of_mass.x - body->pos.x);
    new_vel.y += scalar * (bho->center_of_mass.y - body->pos.y);
    new_vel.z += scalar * (bho->center_of_mass.z - body->pos.z);

    return new_vel;
}

/* frees all nodes in the tree */
void destroy_bhoctree(BHOctree* bho){
    for(int i = 0; i < 8; i++){
        if(bho->sub_sections[i]){   // if != NULL
            destroy_bhoctree(bho->sub_sections[i]);
        }
    }

    free(bho);
}

/*print the octotree*/
void print_bhoctree(BHOctree* bho){
    _print_bhoctree(bho, 0);
}

/* recursive part of print_bhoctree */
void _print_bhoctree(BHOctree *bho, size_t deep){

    switch(bho->status){
        case EMPTY:
            for (size_t i = 0; i < deep; i++){
                printf("| ");
            }
            printf("[ ]\n");

            break;

        case SINGLE_BODY:
            for (size_t i = 0; i < deep; i++){
                printf("| ");
            }
            print_body(bho->body);

            break;

        default:
            for (size_t i = 0; i < deep; i++){
                printf("| ");
            }
            printf("Gnode: %g, %g, %g, %g\n", bho->total_mass, bho->center_of_mass.x, bho->center_of_mass.y, bho->center_of_mass.z);
            for (size_t i = 0; i < 8; i++){

                if (bho->sub_sections[i] != NULL)
                    _print_bhoctree(bho->sub_sections[i], deep + 1);
            }
    }
}