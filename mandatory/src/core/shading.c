/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

bool	is_shadowed(t_world world, t_tuple point, t_light *light)
{
	t_tuple		v;
	float		distance;
	t_tuple		direction;
	t_ray		shadow_ray;

	v = substract_tuple(light->origin, point);
	distance = vector_magnitude(v);
	direction = normalizing_vector(v);
	shadow_ray = (t_ray){point, direction};
	return (intersect_world_shadow(world, shadow_ray, distance));
}

static t_tuple	get_material_contribution(t_comps *comp, t_light *light, \
				bool in_shadow, t_tuple ambient_color)
{
	if (comp->obj->type == OBJ_SPHERE)
		return (lighting(comp->obj->shape.sp.material, *light, comp->point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	else if (comp->obj->type == OBJ_PLANE)
		return (lighting(comp->obj->shape.pl.material, *light, comp->point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	else if (comp->obj->type == OBJ_CYLINDER)
		return (lighting(comp->obj->shape.cy.material, *light, comp->point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	return ((t_tuple){0, 0, 0, 0});
}

static t_tuple	get_ambient_contribution(t_comps *comp, t_tuple ambient_color)
{
	t_material	mat;
	t_tuple		color_at_point;

	if (comp->obj->type == OBJ_SPHERE)
		mat = comp->obj->shape.sp.material;
	else if (comp->obj->type == OBJ_PLANE)
		mat = comp->obj->shape.pl.material;
	else if (comp->obj->type == OBJ_CYLINDER)
		mat = comp->obj->shape.cy.material;
	else
		return ((t_tuple){0, 0, 0, 0});
	color_at_point = mat.color;
	return (hadamard_product(tuple_scalar_mult(color_at_point, mat.ambient), \
		ambient_color));
}

t_tuple	shade_hit(t_world world, t_comps *comp)
{
	t_tuple	final_color;
	t_light	*light;
	bool	in_shadow;
	t_tuple	light_contribution;

	final_color = get_ambient_contribution(comp, world.ambient_color);
	light = world.lights;
	while (light)
	{
		in_shadow = is_shadowed(world, comp->over_point, light);
		light_contribution = get_material_contribution(comp, light, \
			in_shadow, (t_tuple){0, 0, 0, 0});
		final_color = add_tuple(final_color, light_contribution);
		light = light->next;
	}
	final_color.w = 0;
	return (final_color);
}
