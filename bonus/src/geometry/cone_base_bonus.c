/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_base_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by mac               #+#    #+#             */
/*   Updated: 2025/12/23 01:28:19 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
** Create a default cone: unit cone with tip at y=0, base at y=1
** Bounds set to 0-1 in local space (will be scaled by height during transform)
*/
t_cone	cone(void)
{
	return ((t_cone){
		.center = (t_tuple){0, 0, 0, 1},
		.axis = (t_tuple){0, 1, 0, 0},
		.diameter = 2,
		.height = 1,
		.angle = atan(1.0f),
		.trans = identity(),
		.material = material(),
		.minimum = 0.0f,
		.maximum = 1.0f,
		.closed = true,
	});
}

/*
** Compute normal on cone surface in world space
** Normal = normalize(point - nearest_point_on_axis)
** Reference: ahouass approach for geometric normal
*/
t_tuple	cone_local_normal_at(t_cone co, t_tuple world_point)
{
	t_tuple	p;
	t_tuple	point_on_axis;
	float	height;
	float	radius;

	p = substract_tuple(world_point, co.center);
	height = vecs_dot_product(p, co.axis);
	if (co.closed)
	{
		radius = fabsf(height * tanf(co.angle));
		point_on_axis = add_tuple(co.center, tuple_scalar_mult(co.axis, height));
		if (vector_magnitude(substract_tuple(world_point, point_on_axis)) < radius + CONE_EPSILON)
		{
			if (fabsf(height - co.minimum) < CONE_EPSILON)
				return (tuple_scalar_mult(co.axis, -1.0f));
			if (fabsf(height - co.maximum) < CONE_EPSILON)
				return (co.axis);
		}
	}
	point_on_axis = add_tuple(co.center, tuple_scalar_mult(co.axis, height));
	return (normalizing_vector(substract_tuple(world_point, point_on_axis)));
}

/*
** Compute world-space normal at a point on the cone
** Calls cone_local_normal_at which now works in world space
*/
t_tuple	cone_normal_at(t_cone co, t_tuple world_point)
{
	t_tuple	normal;

	normal = cone_local_normal_at(co, world_point);
	if (vector_magnitude(normal) < CONE_EPSILON)
		return (co.axis);
	return (normalizing_vector(normal));
}
