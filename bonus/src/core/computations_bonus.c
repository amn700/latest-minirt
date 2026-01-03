/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computations_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:26:50 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"
#include "../../includes/texture_bonus.h"

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

static t_tuple	get_local_point(t_comps *comp)
{
	if (comp->obj->type == OBJ_SPHERE)
		return (multiply_matrix_by_tuple(
			comp->obj->shape.sp.trans_inv, comp->point));
	else if (comp->obj->type == OBJ_PLANE)
		return (multiply_matrix_by_tuple(
			comp->obj->shape.pl.trans_inv, comp->point));
	else if (comp->obj->type == OBJ_CYLINDER)
		return (multiply_matrix_by_tuple(
			comp->obj->shape.cy.trans_inv, comp->point));
	else if (comp->obj->type == OBJ_CONE)
		return (multiply_matrix_by_tuple(
			comp->obj->shape.co.trans_inv, comp->point));
	return (comp->point);
}

static t_material	get_material(t_comps *comp)
{
	if (comp->obj->type == OBJ_SPHERE)
		return (comp->obj->shape.sp.material);
	else if (comp->obj->type == OBJ_PLANE)
		return (comp->obj->shape.pl.material);
	else if (comp->obj->type == OBJ_CYLINDER)
		return (comp->obj->shape.cy.material);
	else if (comp->obj->type == OBJ_CONE)
		return (comp->obj->shape.co.material);
	return (material());
}

static void	apply_texture_mapping(t_comps *comp)
{
	t_material	mat;
	t_tuple		local_point;

	mat = get_material(comp);
	if (!mat.has_bump_map && !mat.has_normal_map)
		return ;
	local_point = get_local_point(comp);
	if (mat.has_bump_map && mat.bump_map != NULL)
		comp->normalv = perturb_normal_with_bump(mat, comp->normalv,
				local_point, comp->obj->type);
	if (mat.has_normal_map && mat.normal_map != NULL)
		comp->normalv = apply_normal_map(mat, comp->normalv,
				local_point, comp->obj->type);
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
			tuple_scalar_mult(new->normalv, adaptive_epsilon(new->t)));
		new->under_point = substract_tuple(new->point, \
			tuple_scalar_mult(new->normalv, adaptive_epsilon(new->t)));
	}
	else
	{
		new->inside = false;
		new->over_point = add_tuple(new->point, \
			tuple_scalar_mult(original_normal, adaptive_epsilon(new->t)));
		new->under_point = substract_tuple(new->point, \
			tuple_scalar_mult(original_normal, adaptive_epsilon(new->t)));
	}
	new->reflectv = reflect(ray.direction, new->normalv);
	compute_n1_n2(intersection, intersections, new);
	return (new);
}
