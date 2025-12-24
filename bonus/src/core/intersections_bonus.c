/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/22 22:38:24 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

// No cone-specific shadow fast path; use the same flow as other shapes

t_inters	*intersect_world(t_world world, t_ray ray)
{
	t_object	*obj;
	t_inters	*all_intersections;
	t_ray		transformed_ray;

	obj = world.objects;
	all_intersections = NULL;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.sp.trans));
			intersect_sphere(&obj->shape.sp, transformed_ray, \
				&all_intersections, obj);
		}
		else if (obj->type == OBJ_PLANE)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.pl.trans));
			intersect_plane(&obj->shape.pl, transformed_ray, \
				&all_intersections, obj);
		}
		else if (obj->type == OBJ_CYLINDER)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.cy.trans));
			intersect_cylinder(&obj->shape.cy, transformed_ray, \
				&all_intersections, obj);
		}
		else if (obj->type == OBJ_CONE)
		{
			intersect_cone(&obj->shape.co, ray, &all_intersections, obj);
		}
		obj = obj->next;
	}
	sort_intersections(&all_intersections);
	return (all_intersections);
}


bool	intersect_world_shadow(t_world world, t_ray ray, float max_distance)
{
	t_object	*obj;
	t_inters	*intersections;
	t_inters	*current;
	t_inters	*temp;
	t_ray		transformed_ray;

	obj = world.objects;
	intersections = NULL;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.sp.trans));
			intersect_sphere(&obj->shape.sp, transformed_ray, \
				&intersections, obj);
		}
		else if (obj->type == OBJ_PLANE)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.pl.trans));
			intersect_plane(&obj->shape.pl, transformed_ray, \
				&intersections, obj);
		}
		else if (obj->type == OBJ_CYLINDER)
		{
			transformed_ray = transform_ray(ray, \
				inverse_matrix(obj->shape.cy.trans));
			intersect_cylinder(&obj->shape.cy, transformed_ray, \
				&intersections, obj);
		}
		else if (obj->type == OBJ_CONE)
		{
			intersect_cone(&obj->shape.co, ray, &intersections, obj);
		}
		current = intersections;
		while (current)
		{
			// Ignore self-intersections and require a strictly positive hit
			if (current->t > EPSILON && current->t < max_distance)
			{
				while (intersections)
				{
					temp = intersections->next;
					free(intersections);
					intersections = temp;
				}
				return (true);
			}
			current = current->next;
		}
		obj = obj->next;
	}
	while (intersections)
	{
		temp = intersections->next;
		free(intersections);
		intersections = temp;
	}
	return (false);
}
