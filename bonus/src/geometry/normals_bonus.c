#include "../../includes/miniRT_bonus.h"

t_tuple sphere_normal_at(t_sphere sp, t_tuple p)
{
    t_matrix inverse = inverse_matrix(sp.trans);
    t_tuple object_point = multiply_matrix_by_tuple(inverse, p);
    
    // For a unit sphere at origin, normal is just the point vector
    t_tuple object_normal = substract_tuple(object_point, (t_tuple){0,0,0,1});
    
    // Transform normal back to world space
    t_tuple world_normal = multiply_matrix_by_tuple(transposing_matrix(inverse), object_normal);
    world_normal.w = 0;  // Normals are vectors, not points
    
    return normalizing_vector(world_normal);
}
