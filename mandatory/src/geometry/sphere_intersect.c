/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:13:30 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

bool	intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections,
			t_object *obj)
{
	t_quadratic	quad;
	t_tuple		sp_to_ray;
	float		xs[2];

	sp_to_ray = substract_tuple(ray.origin, sp->center);
	quad.a = vecs_dot_product(ray.direction, ray.direction);
	quad.b = vecs_dot_product(ray.direction, sp_to_ray);
	quad.c = vecs_dot_product(sp_to_ray, sp_to_ray) - 1;
	quad.discriminant = (quad.b * quad.b) - (quad.a * quad.c);
	if (quad.discriminant < 0)
		return (false);
	quad.discriminant = sqrt(quad.discriminant);
	xs[0] = (-quad.b - quad.discriminant) / quad.a;
	xs[1] = (-quad.b + quad.discriminant) / quad.a;
	ft_add_intersection(intersections, new_intersection(xs[0], obj));
	ft_add_intersection(intersections, new_intersection(xs[1], obj));
	sort_intersections(intersections);
	return (true);
}
