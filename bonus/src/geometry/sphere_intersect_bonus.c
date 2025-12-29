/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersect_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 19:46:10 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

bool	intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections, \
		t_object *obj)
{
	t_tuple		sp_to_ray;
	float		a;
	float		b_half;
	float		c;
	float		discriminant;
	float		xs[2];

	sp_to_ray = substract_tuple(ray.origin, sp->center);
	a = vecs_dot_product(ray.direction, ray.direction);
	b_half = vecs_dot_product(ray.direction, sp_to_ray);
	c = vecs_dot_product(sp_to_ray, sp_to_ray) - 1;
	discriminant = (b_half * b_half) - (a * c);
	if (discriminant < 0)
		return (false);
	discriminant = sqrt(discriminant);
	xs[0] = (-b_half - discriminant) / a;
	xs[1] = (-b_half + discriminant) / a;
	ft_add_intersection(intersections, new_intersection(xs[0], obj));
	ft_add_intersection(intersections, new_intersection(xs[1], obj));
	sort_intersections(intersections);
	return (true);
}
