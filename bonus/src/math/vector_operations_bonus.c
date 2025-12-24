/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:16:40 by mohchaib          #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/* Pythagoras’ theorem
	∣v∣=sqrt(x.2+y.2+z.2)k
*/

//magnitude
float	vector_magnitude(t_tuple v)
{
	return (sqrtf((v.x * v.x) + (v.y * v.y) + (v.z *v.z)));
}
//normilization

t_tuple	normalizing_vector(t_tuple v)
{
	float mag = vector_magnitude(v);
	if (mag == 0.0f)
        return (t_tuple){0,0,0,0};
	return (t_tuple){(v.x / mag), (v.y / mag), (v.z / mag), v.w};
}
/* Dot Product is a way of combining two vectors to get a single number
that represent as far as i know the angle between them ??
	a⋅b= ax ​bx ​+ ay ​by ​+ az​ bz​
*/
float	vecs_dot_product(t_tuple va, t_tuple vb)
{
	return ((va.x * vb.x) + (va.y * vb.y) + (va.z * vb.z));
}
/*cross_ product 
	a×b=(ay​bz​−az​by​,az​bx​−ax​bz​,ax​by​−ay​bx​)
*/
t_tuple	vecs_cross_product(t_tuple va, t_tuple vb)
{
	return (t_tuple) {(va.y * vb.z) - (va.z * vb.y) , (va.z * vb.x) - (va.x * vb.z), (va.x * vb.y) - (va.y * vb.x), 0};
}

t_tuple hadamard_product(t_tuple a, t_tuple b)
{
    return (t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, 0};
}



/* Geometric interpretation
	a⋅b=∣a∣∣b∣cosθ
*/
