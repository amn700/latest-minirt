/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/15 18:36:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_material	material(void)
{
	return ((t_material){
		.color = {1, 1, 1, 0},
		.ambient = 0.3,
		.diffuse = 0.9,
		.specular = 0.9,
		.reflective = 0.0,
		.shininess = 200.0,
		.transparency = 0.0,
		.refract_index = 1.0
	});
}

t_sphere	sphere(void)
{
	return ((t_sphere){
		.trans = identity(),
		.diam = 1,
		.center = (t_tuple){0, 0, 0, 1},
		.material = material()
	});
}

t_sphere	sp_transform(t_matrix m, t_sphere sp)
{
	t_tuple	new_center;

	new_center = multiply_matrix_by_tuple(m, sp.center);
	return ((t_sphere){
		.diam = sp.diam,
		.center = new_center,
		.material = sp.material,
		.trans = identity()
	});
}

t_tuple	sphere_normal_at(t_sphere sp, t_tuple p)
{
	t_matrix	inverse;
	t_tuple		object_point;
	t_tuple		object_normal;
	t_tuple		world_normal;

	inverse = inverse_matrix(sp.trans);
	object_point = multiply_matrix_by_tuple(inverse, p);
	object_normal = substract_tuple(object_point, (t_tuple){0, 0, 0, 1});
	world_normal = multiply_matrix_by_tuple(transposing_matrix(inverse), \
		object_normal);
	world_normal.w = 0;
	return (normalizing_vector(world_normal));
}
