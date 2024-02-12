#include "bounding_box.h"

/*set the bound of a new box*/
BoundingBox init_box(double min_x, double max_x,
                     double min_y, double max_y,
                     double min_z, double max_z,
                     double side){
    BoundingBox box;

    box.side = side;
    box.min_x = min_x;
    box.max_x = max_x;
    box.min_y = min_y;
    box.max_y = max_y;
    box.min_z = min_z;
    box.max_z = max_z;
    box.mid_x = (max_x + min_x) * 0.5;
    box.mid_x = (max_y + min_y) * 0.5;
    box.mid_x = (max_z + min_z) * 0.5;

    return box;
}

/*create a new sub box in the original box*/
BoundingBox init_subbox(BoundingBox box, bool upper, bool north, bool west){

    if(upper)   box.max_y = box.mid_y;
    else        box.min_y = box.mid_y;

    if(north)   box.max_z = box.mid_z;
    else        box.min_z = box.mid_z;

    if(west)    box.max_x = box.mid_x;
    else        box.min_x = box.mid_x;

    box.mid_x = (box.max_x + box.min_x) * 0.5;
    box.mid_y = (box.max_y + box.min_y) * 0.5;
    box.mid_z = (box.max_z + box.min_z) * 0.5;
    box.side *= 0.5;
    return box;
}

/*return true if the poin is in the bound of the box, false otherwise */
bool is_inside_box(BoundingBox* box, Vec3D* point){
    if(point->x < box->min_x || point->x > box->max_x)    return false;
    if(point->y < box->min_y || point->y > box->max_y)    return false;
    if(point->z < box->min_z || point->z > box->max_z)    return false;

    return true;
}

/*print a bounding box*/
void print_box(BoundingBox* box){
    printf("{(%g, %g, %g), (%g, %g, %g), (%g, %g, %g)}\n",
        box->min_x, box->mid_x, box->max_x,
        box->min_y, box->mid_y, box->max_y,
        box->min_z, box->mid_z, box->max_z);
}

