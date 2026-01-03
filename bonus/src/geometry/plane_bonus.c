#include "../../includes/miniRT_bonus.h"

void	ft_add_intersection(t_inters **lst, t_inters *new);
t_inters	*new_intersection(float t, t_object *obj);
t_plane plane(void)
{
    return (t_plane){
        .point = (t_tuple){0,0,0,1},
        .normal = (t_tuple){0,1,0,0},
        .trans = identity(),
        .trans_inv = identity(),
        .material = material(),
    };
}

t_tuple plane_normal_at(t_plane pl)
{
    t_tuple object_normal = (t_tuple){0, 1, 0, 0};
    t_tuple world_normal = multiply_matrix_by_tuple(transposing_matrix(pl.trans_inv), object_normal);
    world_normal.w = 0;
    
    return normalizing_vector(world_normal);
}

bool    intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections, \
		t_object *obj)
{
    t_inters *new_inters;
    float t;

    (void)pl;
    // In object space, plane is at y=0 with normal (0,1,0)
    // Check if ray is parallel to the plane
    if (fabs(ray.direction.y) < 1e-6)
        return true; // No intersections, ray is parallel
    
    // Calculate intersection t where ray hits y=0
    t = -ray.origin.y / ray.direction.y;
    
    new_inters = new_intersection(t, obj);
    if (!new_inters)
        return false;
    
    ft_add_intersection(intersections, new_inters);
    return true;
}

// bool aio_intersect(t_object *obj, t_ray ray, t_inters **intersections)
// {
//     if (obj->type == OBJ_PLANE)
//         return intersect_plane(&obj->shape.pl, ray, intersections);
//     else if (obj->type == OBJ_SPHERE)
//         return intersect_sphere(&obj->shape.sp, ray, intersections);
//     else if (obj->type == OBJ_CYLINDER)
//         return intersect_cylinder(&obj->shape.cy, ray, intersections);
//     return true;
// }