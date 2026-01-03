/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:28:00 by amn              ###   ########.fr       */
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

void	comp_init(t_inters *intersection, t_ray ray, t_comps *new)
{
	new->t = intersection->t;
	new->obj = intersection->object;
	new->point = position(ray, new->t);
	new->eyev = negate_tuple(ray.direction);
}

t_comps	*prepare_computations(t_inters *intersection, t_ray ray)
{
	t_comps	*new;
	t_tuple	original_normal;

	new = new_comps();
	if (!new)
		return (NULL);
	comp_init(intersection, ray, new);
	get_normal_vector(new);
	original_normal = new->normalv;
	if (vecs_dot_product(new->normalv, new->eyev) < 0)
	{
		new->inside = true;
		new->normalv = negate_tuple(new->normalv);
		new->over_point = add_tuple(new->point, tuple_scalar_mult(new->normalv,
					EPSILON));
	}
	else
	{
		new->inside = false;
		new->over_point = add_tuple(new->point,
				tuple_scalar_mult(original_normal, EPSILON));
	}
	return (new);
}
