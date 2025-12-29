/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

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
	t_tuple		point;
	t_material	mat;

	point = comp->point;
	if (comp->obj->type == OBJ_SPHERE)
	{
		point = multiply_matrix_by_tuple(inverse_matrix( \
			comp->obj->shape.sp.trans), comp->point);
		mat = comp->obj->shape.sp.material;
		if (mat.has_color_texture && mat.color_texture != NULL)
			mat.color = (t_tuple){1, 1, 1, 0};  // Use white for texture objects
		return (lighting(mat, *light, point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	}
	else if (comp->obj->type == OBJ_PLANE)
	{
		point = multiply_matrix_by_tuple(inverse_matrix( \
			comp->obj->shape.pl.trans), comp->point);
		mat = comp->obj->shape.pl.material;
		if (mat.has_color_texture && mat.color_texture != NULL)
			mat.color = (t_tuple){1, 1, 1, 0};  // Use white for texture objects
		return (lighting(mat, *light, point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	}
	else if (comp->obj->type == OBJ_CYLINDER)
	{
		point = multiply_matrix_by_tuple(inverse_matrix(comp->obj->shape.cy.trans), comp->point);
		mat = comp->obj->shape.cy.material;
		if (mat.has_color_texture && mat.color_texture != NULL)
			mat.color = (t_tuple){1, 1, 1, 0};  // Use white for texture objects
		return (lighting(mat, *light, point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	}
	else if (comp->obj->type == OBJ_CONE)
	{
		point = multiply_matrix_by_tuple(inverse_matrix(comp->obj->shape.co.trans), comp->point);
		mat = comp->obj->shape.co.material;
		if (mat.has_color_texture && mat.color_texture != NULL)
			mat.color = (t_tuple){1, 1, 1, 0};  // Use white for texture objects
		return (lighting(mat, *light, point, \
			comp->eyev, comp->normalv, in_shadow, ambient_color));
	}
	return ((t_tuple){0, 0, 0, 0});
}

t_tuple reflected_color(t_world world, t_comps *comps, int depth);
t_tuple refracted_color(t_world world, t_comps *comps, int depth);

static t_tuple	get_ambient_contribution(t_comps *comp, t_tuple ambient_color)
{
	t_material	mat;
	t_tuple		color_at_point;
	t_tuple		pos;

	if (comp->obj->type == OBJ_SPHERE)
		mat = comp->obj->shape.sp.material;
	else if (comp->obj->type == OBJ_PLANE)
		mat = comp->obj->shape.pl.material;
	else if (comp->obj->type == OBJ_CYLINDER)
		mat = comp->obj->shape.cy.material;
	else if (comp->obj->type == OBJ_CONE)
		mat = comp->obj->shape.co.material;
	else
		return ((t_tuple){0, 0, 0, 0});
	
	// Use white for textured objects, texture color applied later in shade_hit
	if (mat.has_color_texture && mat.color_texture != NULL)
		color_at_point = (t_tuple){1, 1, 1, 0};
	else
		color_at_point = mat.color;
	
	// Apply pattern if present (for non-textured objects)
	if (mat.pattern.at != NULL)
	{
		pos = comp->point;
		if (comp->obj->type == OBJ_SPHERE)
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.sp.trans), pos);
		else if (comp->obj->type == OBJ_PLANE)
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.pl.trans), pos);
		else if (comp->obj->type == OBJ_CYLINDER)
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.cy.trans), pos);
		else if (comp->obj->type == OBJ_CONE)
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.co.trans), pos);
		if (mat.pattern.has_transform)
			pos = multiply_matrix_by_tuple(mat.pattern.inv_transform, pos);
		color_at_point = mat.pattern.at(mat.pattern, pos);
	}
	return (hadamard_product(tuple_scalar_mult(color_at_point, mat.ambient), \
		ambient_color));
}

static float	get_object_transparency(t_comps *comp)
{
	if (comp->obj->type == OBJ_SPHERE)
		return (comp->obj->shape.sp.material.transparency);
	else if (comp->obj->type == OBJ_PLANE)
		return (comp->obj->shape.pl.material.transparency);
	else if (comp->obj->type == OBJ_CYLINDER)
		return (comp->obj->shape.cy.material.transparency);
	else if (comp->obj->type == OBJ_CONE)
		return (comp->obj->shape.co.material.transparency);
	return (0.0);
}

static float	schlick(t_comps *comps)
{
	float	cos;
	float	n;
	float	sin2_t;
	float	cos_t;
	float	r0;

	cos = vecs_dot_product(comps->eyev, comps->normalv);
	if (comps->n1 > comps->n2)
	{
		n = comps->n1 / comps->n2;
		sin2_t = n * n * (1.0 - cos * cos);
		if (sin2_t > 1.0)
			return (1.0);
		cos_t = sqrtf(1.0 - sin2_t);
		cos = cos_t;
	}
	r0 = ((comps->n1 - comps->n2) / (comps->n1 + comps->n2));
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * powf((1.0 - cos), 5));
}

t_tuple	shade_hit(t_world world, t_comps *comp, int depth)
{
	t_tuple	surface;
	t_tuple	opaque_surface;
	t_tuple	reflected;
	t_tuple	refracted;
	t_light	*light;
	bool	in_shadow;
	t_tuple	light_contrib;
	float	transparency;
	float	reflectance;
	t_material mat;
	t_tuple texture_color;
	t_tuple pos;
	t_tuple uv;

	// Initialize mat with default values to prevent undefined behavior
	mat = material();
	mat.has_color_texture = false;
	mat.color_texture = NULL;

	// Get texture color if available
	texture_color = (t_tuple){1, 1, 1, 0};  // White by default (no texture)
	if (comp->obj->type == OBJ_SPHERE)
		mat = comp->obj->shape.sp.material;
	else if (comp->obj->type == OBJ_PLANE)
		mat = comp->obj->shape.pl.material;
	else if (comp->obj->type == OBJ_CYLINDER)
		mat = comp->obj->shape.cy.material;
	else if (comp->obj->type == OBJ_CONE)
		mat = comp->obj->shape.co.material;
	
	// Sample texture if present
	if (mat.has_color_texture && mat.color_texture != NULL)
	{
		pos = comp->point;
		if (comp->obj->type == OBJ_SPHERE)
		{
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.sp.trans), pos);
			uv = sphere_uv_mapping(pos);
		}
		else if (comp->obj->type == OBJ_PLANE)
		{
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.pl.trans), pos);
			uv = plane_uv_mapping(pos);
		}
		else if (comp->obj->type == OBJ_CYLINDER)
		{
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.cy.trans), pos);
			uv = cylinder_uv_mapping(pos);
		}
		else if (comp->obj->type == OBJ_CONE)
		{
			pos = multiply_matrix_by_tuple(inverse_matrix( \
				comp->obj->shape.co.trans), pos);
			uv = cone_uv_mapping(pos);
		}
		texture_color = sample_color_from_texture(mat.color_texture, uv.x, uv.y);
	}

	opaque_surface = get_ambient_contribution(comp, world.ambient_color);
	light = world.lights;
	while (light)
	{
		in_shadow = is_shadowed(world, comp->over_point, light);
		light_contrib = get_material_contribution(comp, light, \
			in_shadow, (t_tuple){0, 0, 0, 0});
		opaque_surface = add_tuple(opaque_surface, light_contrib);
		light = light->next;
	}
	// Apply texture color to the final lit surface
	// Pure hadamard product: modulates lighting by texture color
	opaque_surface = hadamard_product(opaque_surface, texture_color);
	transparency = get_object_transparency(comp);
	surface = tuple_scalar_mult(opaque_surface, 1.0 - transparency);
	reflected = reflected_color(world, comp, depth);
	refracted = refracted_color(world, comp, depth);
	if (reflected.x > 0.0 && refracted.x > 0.0)
	{
		reflectance = schlick(comp);
		surface = add_tuple(surface, tuple_scalar_mult(reflected, reflectance));
		surface = add_tuple(surface, tuple_scalar_mult(refracted, \
			(1.0 - reflectance)));
	}
	else
	{
		surface = add_tuple(surface, reflected);
		surface = add_tuple(surface, refracted);
	}
	surface.w = 0;
	return (surface);
}
