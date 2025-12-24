#include "../../includes/miniRT_bonus.h"

t_tuple reflect(t_tuple in, t_tuple normal)
{
    // in - (normal * 2 * dot(in, normal))
    return substract_tuple(in , tuple_scalar_mult(normal , 2 * vecs_dot_product(in, normal)));
}