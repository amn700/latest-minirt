/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersect_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by mac               #+#    #+#             */
/*   Updated: 2025/12/23 01:28:19 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
** Add cone intersection if the height along axis is within bounds
** Uses world-space height projection like ahouass reference
*/
static void	add_if_in_bounds(t_cone *cone, t_object *obj,
	float t, t_ray ray, t_inters **xs)
{
	float		height;
	t_tuple		point;
	t_inters	*inter;

	if (t < CONE_EPSILON)
		return ;
	point = position(ray, t);
	height = vecs_dot_product(cone->axis, substract_tuple(point, cone->center));
	if (height >= cone->minimum && height <= cone->maximum)
	{
		inter = new_intersection(t, obj);
		if (inter)
			ft_add_intersection(xs, inter);
	}
}

/*
** Check if intersection at t hits within the cap radius
** For cone at height h, radius = h * tan(angle)
*/
static bool	check_cone_cap(t_cone *cone, t_ray ray, float t, float h_cap)
{
	t_tuple	point;
	t_tuple	cap_center;
	float	radius;
	float	dist;

	point = position(ray, t);
	cap_center = add_tuple(cone->center, tuple_scalar_mult(cone->axis, h_cap));
	radius = fabsf(h_cap * tanf(cone->angle));
	dist = vector_magnitude(substract_tuple(point, cap_center));
	return (dist <= radius + CONE_EPSILON);
}

/*
** Add intersections for cone end caps
** Uses world-space plane intersection for caps
*/
static void	add_cone_caps(t_cone *cone, t_object *obj,
	t_ray ray, t_inters **xs)
{
	float		t;
	float		denom;
	t_tuple		cap_center;
	t_tuple		to_cap;
	t_inters	*inter;

	if (!cone->closed)
		return ;
	denom = vecs_dot_product(ray.direction, cone->axis);
	if (fabsf(denom) < CONE_EPSILON)
		return ;
	cap_center = add_tuple(cone->center, tuple_scalar_mult(cone->axis, cone->minimum));
	to_cap = substract_tuple(cap_center, ray.origin);
	t = vecs_dot_product(to_cap, cone->axis) / denom;
	if (t > CONE_EPSILON && check_cone_cap(cone, ray, t, cone->minimum))
	{
		inter = new_intersection(t, obj);
		if (inter)
			ft_add_intersection(xs, inter);
	}
	cap_center = add_tuple(cone->center, tuple_scalar_mult(cone->axis, cone->maximum));
	to_cap = substract_tuple(cap_center, ray.origin);
	t = vecs_dot_product(to_cap, cone->axis) / denom;
	if (t > CONE_EPSILON && check_cone_cap(cone, ray, t, cone->maximum))
	{
		inter = new_intersection(t, obj);
		if (inter)
			ft_add_intersection(xs, inter);
	}
}

/*
** Intersect ray with cone surface using angle-based formula
** Reference: ahouass/42-miniRT angle-based intersection
** k = tan²(angle), encodes cone geometry directly
*/
bool	intersect_cone(t_cone *cone, t_ray ray, t_inters **xs, t_object *obj)
{
	float	k;
	float	a;
	float	b;
	float	c;
	float	disc;
	float	t[2];
	t_tuple	co;

	co = substract_tuple(ray.origin, cone->center);
	k = tanf(cone->angle) * tanf(cone->angle);
	a = vecs_dot_product(ray.direction, ray.direction)
		- (1.0f + k) * powf(vecs_dot_product(ray.direction, cone->axis), 2.0f);
	b = 2.0f * (vecs_dot_product(ray.direction, co)
		- (1.0f + k) * vecs_dot_product(ray.direction, cone->axis)
		* vecs_dot_product(co, cone->axis));
	c = vecs_dot_product(co, co)
		- (1.0f + k) * powf(vecs_dot_product(co, cone->axis), 2.0f);
	if (fabsf(a) < CONE_EPSILON)
	{
		/* Linear case: a≈0 => solve b*t + c = 0 */
		if (fabsf(b) >= CONE_EPSILON)
			add_if_in_bounds(cone, obj, -c / b, ray, xs);
		add_cone_caps(cone, obj, ray, xs);
		return (true);
	}
	disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
		return (add_cone_caps(cone, obj, ray, xs), true);
	t[0] = (-b - sqrtf(disc)) / (2.0f * a);
	t[1] = (-b + sqrtf(disc)) / (2.0f * a);
	add_if_in_bounds(cone, obj, t[0], ray, xs);
	add_if_in_bounds(cone, obj, t[1], ray, xs);
	add_cone_caps(cone, obj, ray, xs);
	return (true);
}
