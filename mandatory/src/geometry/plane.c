/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:25:56 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void		ft_add_intersection(t_inters **lst, t_inters *new);
t_inters	*new_intersection(float t, t_object *obj);

t_plane	plane(void)
{
	t_plane	pl;

	pl.point = (t_tuple){0, 0, 0, 1};
	pl.normal = (t_tuple){0, 1, 0, 0};
	pl.trans = identity();
	pl.material = material();
	return (pl);
}

t_tuple	plane_normal_at(t_plane pl)
{
	t_tuple		object_normal;
	t_matrix	inv;
	t_tuple		world_normal;

	object_normal = (t_tuple){0, 1, 0, 0};
	inv = inverse_matrix(pl.trans);
	world_normal = multiply_matrix_by_tuple(transposing_matrix(inv),
			object_normal);
	world_normal.w = 0;
	return (normalizing_vector(world_normal));
}

bool	intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections,
		t_object *obj)
{
	float		t;
	t_inters	*new_inters;

	(void)pl;
	if (fabs(ray.direction.y) < 1e-6)
		return (true);
	t = -ray.origin.y / ray.direction.y;
	new_inters = new_intersection(t, obj);
	if (!new_inters)
		return (false);
	ft_add_intersection(intersections, new_inters);
	return (true);
}
