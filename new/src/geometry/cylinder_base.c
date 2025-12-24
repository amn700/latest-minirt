/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_cylinder	cylinder(void)
{
	return ((t_cylinder){
		.center = (t_tuple){0, 0, 0, 1},
		.axis = (t_tuple){0, 1, 0, 0},
		.diameter = 2,
		.height = INFINITY,
		.trans = identity(),
		.material = material(),
		.minimum = -INFINITY,
		.maximum = INFINITY,
	});
}

t_tuple	cylinder_normal_at(t_cylinder cyl, t_tuple point)
{
	t_matrix	inverse;
	t_tuple		object_point;
	t_tuple		object_normal;
	t_tuple		world_normal;

	inverse = inverse_matrix(cyl.trans);
	object_point = multiply_matrix_by_tuple(inverse, point);
	object_normal = (t_tuple){object_point.x, 0, object_point.z, 0};
	world_normal = multiply_matrix_by_tuple(transposing_matrix(inverse),
			object_normal);
	world_normal.w = 0;
	return (normalizing_vector(world_normal));
}
