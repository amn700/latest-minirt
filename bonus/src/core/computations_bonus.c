/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computations_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/20 06:40:52 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static t_comps	*new_comps(void)
{
	t_comps	*comp;

	comp = malloc(sizeof(t_comps));
	if (!comp)
		return (NULL);
	comp->eyev = (t_tuple){0, 0, 0, 0};
	comp->normalv = (t_tuple){0, 0, 0, 0};
	comp->point = (t_tuple){0, 0, 0, 1};
	comp->over_point = (t_tuple){0, 0, 0, 1};
	comp->under_point = (t_tuple){0, 0, 0, 1};
	comp->t = 0.0;
	comp->obj = NULL;
	comp->inside = false;
	comp->n1 = 1.0;
	comp->n2 = 1.0;
	return (comp);
}

static void	get_normal_vector(t_comps *new)
{
	if (new->obj->type == OBJ_SPHERE)
		new->normalv = sphere_normal_at(new->obj->shape.sp, new->point);
	else if (new->obj->type == OBJ_PLANE)
		new->normalv = plane_normal_at(new->obj->shape.pl);
	else if (new->obj->type == OBJ_CYLINDER)
		new->normalv = cylinder_normal_at(new->obj->shape.cy, new->point);
	else if (new->obj->type == OBJ_CONE)
		new->normalv = cone_normal_at(new->obj->shape.co, new->point);
}

static void	apply_texture_mapping(t_comps *new)
{
	t_material	material;
	t_tuple		local_point;
	t_matrix	inverse;

	if (new->obj->type == OBJ_SPHERE)
	{
		material = new->obj->shape.sp.material;
		inverse = inverse_matrix(new->obj->shape.sp.trans);
		local_point = multiply_matrix_by_tuple(inverse, new->point);
		if (material.has_normal_map)
			new->normalv = apply_normal_map(material, new->normalv, local_point, OBJ_SPHERE);
		else if (material.has_bump_map)
			new->normalv = perturb_normal_with_bump(material, new->normalv, local_point, OBJ_SPHERE);
	}
	else if (new->obj->type == OBJ_PLANE)
	{
		material = new->obj->shape.pl.material;
		inverse = inverse_matrix(new->obj->shape.pl.trans);
		local_point = multiply_matrix_by_tuple(inverse, new->point);
		if (material.has_normal_map)
			new->normalv = apply_normal_map(material, new->normalv, local_point, OBJ_PLANE);
		else if (material.has_bump_map)
			new->normalv = perturb_normal_with_bump(material, new->normalv, local_point, OBJ_PLANE);
	}
	else if (new->obj->type == OBJ_CYLINDER)
	{
		material = new->obj->shape.cy.material;
		inverse = inverse_matrix(new->obj->shape.cy.trans);
		local_point = multiply_matrix_by_tuple(inverse, new->point);
		if (material.has_normal_map)
			new->normalv = apply_normal_map(material, new->normalv, local_point, OBJ_CYLINDER);
		else if (material.has_bump_map)
			new->normalv = perturb_normal_with_bump(material, new->normalv, local_point, OBJ_CYLINDER);
	}
	else if (new->obj->type == OBJ_CONE)
	{
		material = new->obj->shape.co.material;
		inverse = inverse_matrix(new->obj->shape.co.trans);
		local_point = multiply_matrix_by_tuple(inverse, new->point);
		if (material.has_normal_map)
			new->normalv = apply_normal_map(material, new->normalv, local_point, OBJ_CONE);
		else if (material.has_bump_map)
			new->normalv = perturb_normal_with_bump(material, new->normalv, local_point, OBJ_CONE);
	}
}

t_comps	*prepare_computations(t_inters *intersection, t_ray ray, t_inters *intersections)
{
	t_comps	*new;
	t_tuple	original_normal;

	new = new_comps();
	if (!new)
		return (NULL);
	new->t = intersection->t;
	new->obj = intersection->object;
	new->point = position(ray, new->t);
	new->eyev = negate_tuple(ray.direction);
	get_normal_vector(new);
	apply_texture_mapping(new);
	original_normal = new->normalv;
	if (vecs_dot_product(new->normalv, new->eyev) < 0)
	{
		new->inside = true;
		new->normalv = negate_tuple(new->normalv);
		new->over_point = add_tuple(new->point, \
			tuple_scalar_mult(new->normalv, EPSILON));
		new->under_point = substract_tuple(new->point, \
			tuple_scalar_mult(new->normalv, EPSILON));
	}
	else
	{
		new->inside = false;
		new->over_point = add_tuple(new->point, \
			tuple_scalar_mult(original_normal, EPSILON));
		new->under_point = substract_tuple(new->point, \
			tuple_scalar_mult(original_normal, EPSILON));
	}
	new->reflectv = reflect(ray.direction, new->normalv);
	compute_n1_n2(intersection, intersections, new);
	return (new);
}
