/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/09 07:55:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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
	comp->t = 0.0;
	comp->obj = NULL;
	comp->inside = false;
	comp->u = 0.0;
	comp->v = 0.0;
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
}

static void	compute_uv_coordinates(t_comps *new, t_ray ray)
{
	t_tuple	local_point;
	t_matrix	inv;

	if (new->obj->type == OBJ_SPHERE)
	{
		inv = inverse_matrix(new->obj->shape.sp.trans);
		local_point = multiply_matrix_by_tuple(inv, new->point);
		sphere_uv(local_point, &new->u, &new->v);
	}
	else if (new->obj->type == OBJ_PLANE)
	{
		inv = inverse_matrix(new->obj->shape.pl.trans);
		local_point = multiply_matrix_by_tuple(inv, new->point);
		plane_uv(local_point, &new->u, &new->v);
	}
	else if (new->obj->type == OBJ_CYLINDER)
	{
		inv = inverse_matrix(new->obj->shape.cy.trans);
		local_point = multiply_matrix_by_tuple(inv, new->point);
		cylinder_uv(local_point, &new->u, &new->v);
	}
	(void)ray;
}

t_comps	*prepare_computations(t_inters *intersection, t_ray ray)
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
	compute_uv_coordinates(new, ray);
	new->normalv = perturb_normal_bump(new, new->normalv);
	original_normal = new->normalv;
	if (vecs_dot_product(new->normalv, new->eyev) < 0)
	{
		new->inside = true;
		new->normalv = negate_tuple(new->normalv);
		new->over_point = add_tuple(new->point, \
			tuple_scalar_mult(new->normalv, EPSILON));
	}
	else
	{
		new->inside = false;
		new->over_point = add_tuple(new->point, \
			tuple_scalar_mult(original_normal, EPSILON));
	}
	return (new);
}
