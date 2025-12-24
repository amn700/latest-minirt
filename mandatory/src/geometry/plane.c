#include "../../includes/miniRT.h"

void	ft_add_intersection(t_inters **lst, t_inters *new);
t_inters	*new_intersection(float t, t_object *obj);
t_plane plane(void)
{
    return (t_plane){
        .point = (t_tuple){0,0,0,1},
        .normal = (t_tuple){0,1,0,0},
        .trans = identity(),
        .material = material(),
    };
}

t_tuple plane_normal_at(t_plane pl)
{
    // Plane normal in object space is always (0, 1, 0)
    t_tuple object_normal = (t_tuple){0, 1, 0, 0};
    
    // Transform the normal using the inverse transpose
    t_matrix inverse = inverse_matrix(pl.trans);
    t_tuple world_normal = multiply_matrix_by_tuple(transposing_matrix(inverse), object_normal);
    world_normal.w = 0; // Ensure it's a vector
    
    return normalizing_vector(world_normal);
}

bool    intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections, \
		t_object *obj)
{
    (void)pl;
    // Check if ray is parallel to the plane
    if (fabs(ray.direction.y) < 1e-6)
        return true; // No intersections, ray is parallel
    
    // Calculate intersection t
    float t = -ray.origin.y / ray.direction.y;

    t_inters *new_inters = new_intersection(t, obj);
    if (!new_inters)
        return false;
    
    ft_add_intersection(intersections, new_inters);
    return true;
}