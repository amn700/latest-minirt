/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:14:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	swap_floats(float *a, float *b)
{
	float	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static bool	add_cylinder_intersection(t_object *obj, float t, float y,
		t_inters **intersections)
{
	t_inters	*inters;

	if (y <= obj->shape.cy.minimum || y >= obj->shape.cy.maximum)
		return (false);
	inters = new_intersection(t, obj);
	if (!inters)
		return (false);
	ft_add_intersection(intersections, inters);
	return (true);
}

bool	intersect_cylinder(t_cylinder *cyl, t_ray ray, t_inters **intersections,
		t_object *obj)
{
	t_quadratic	quad;
	float		t[2];
	float		y[2];

	(void)cyl;
	quad.a = ray.direction.x * ray.direction.x + ray.direction.z
		* ray.direction.z;
	if (fabs(quad.a) < 1e-6)
		return (true);
	quad.b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z
			* ray.direction.z);
	quad.c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
	quad.discriminant = quad.b * quad.b - 4 * quad.a * quad.c;
	if (quad.discriminant < 0)
		return (true);
	t[0] = (-quad.b - sqrt(quad.discriminant)) / (2 * quad.a);
	t[1] = (-quad.b + sqrt(quad.discriminant)) / (2 * quad.a);
	if (t[0] > t[1])
		swap_floats(&t[0], &t[1]);
	y[0] = ray.origin.y + t[0] * ray.direction.y;
	y[1] = ray.origin.y + t[1] * ray.direction.y;
	add_cylinder_intersection(obj, t[0], y[0], intersections);
	add_cylinder_intersection(obj, t[1], y[1], intersections);
	return (true);
}
