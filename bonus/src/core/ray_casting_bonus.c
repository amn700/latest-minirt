/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/09 10:29:46 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	cleanup_intersections(t_inters *intersections)
{
	t_inters	*cur;
	t_inters	*tmp;

	cur = intersections;
	while (cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

static t_inters	*find_hit_intersection(t_inters *intersections, float hit_t)
{
	t_inters	*hit_i;

	hit_i = intersections;
	while (hit_i && !f_equal(hit_i->t, hit_t))
		hit_i = hit_i->next;
	return (hit_i);
}

t_tuple	color_at(t_world world, t_ray ray, int depth)
{
	t_inters	*intersections;
	float		hit_t;
	t_inters	*hit_i;
	t_comps		*comp;
	t_tuple		color;

	if (depth <= 0)
		return ((t_tuple){0, 0, 0, 0});
	intersections = intersect_world(world, ray);
	if (!intersections)
		return ((t_tuple){0, 0, 0, 0});
	hit_t = hit(intersections);
	if (hit_t < 0)
	{
		cleanup_intersections(intersections);
		return ((t_tuple){0, 0, 0, 0});
	}
	hit_i = find_hit_intersection(intersections, hit_t);
	comp = prepare_computations(hit_i, ray, intersections);
	color = shade_hit(world, comp, depth);
	free(comp);
	cleanup_intersections(intersections);
	return (color);
}
