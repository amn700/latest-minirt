#include "../../includes/miniRT_bonus.h"

t_tuple sphere_normal_at(t_sphere sp, t_tuple p)
{
    t_tuple object_point = multiply_matrix_by_tuple(sp.trans_inv, p);
    t_tuple object_normal = substract_tuple(object_point, (t_tuple){0,0,0,1});
    t_tuple world_normal = multiply_matrix_by_tuple(transposing_matrix(sp.trans_inv), object_normal);
    world_normal.w = 0;
    
    return normalizing_vector(world_normal);
}
