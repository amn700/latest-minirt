/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:40:10 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

//a function that return the correct transformed ray for all objects
t_ray	correct_ray(t_object *obj, t_ray ray)
{
	t_ray		transformed_ray;

	if (obj->type == OBJ_SPHERE)
		transformed_ray = transform_ray(ray,
				inverse_matrix(obj->shape.sp.trans));
	else if (obj->type == OBJ_PLANE)
		transformed_ray = transform_ray(ray,
				inverse_matrix(obj->shape.pl.trans));
	else if (obj->type == OBJ_CYLINDER)
		transformed_ray = transform_ray(ray,
				inverse_matrix(obj->shape.cy.trans));
	return (transformed_ray);
}

t_inters	*intersect_world(t_world world, t_ray ray)
{
	t_object	*obj;
	t_inters	*all_intersections;
	t_ray		transformed_ray;

	obj = world.objects;
	all_intersections = NULL;
	while (obj)
	{
		transformed_ray = correct_ray(obj, ray);
		if (obj->type == OBJ_SPHERE)
			intersect_sphere(&obj->shape.sp, transformed_ray,
				&all_intersections, obj);
		else if (obj->type == OBJ_PLANE)
			intersect_plane(&obj->shape.pl, transformed_ray, &all_intersections,
				obj);
		else if (obj->type == OBJ_CYLINDER)
			intersect_cylinder(&obj->shape.cy, transformed_ray,
				&all_intersections, obj);
		obj = obj->next;
	}
	sort_intersections(&all_intersections);
	return (all_intersections);
}

void	free_intersections(t_inters *intersections)
{
	t_inters	*temp;

	while (intersections)
	{
		temp = intersections->next;
		free(intersections);
		intersections = temp;
	}
}

bool	inter_check(t_inters *intersections, t_inters *current,
			float max_distance)
{
	while (current)
	{
		if (current->t >= 0 && current->t < max_distance)
			return (free_intersections(intersections), true);
		current = current->next;
	}
	return (false);
}

bool	intersect_world_shadow(t_world world, t_ray ray, float max_distance)
{
	t_object	*obj;
	t_inters	*ints;
	t_inters	*current;

	obj = world.objects;
	ints = NULL;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
			intersect_sphere(&obj->shape.sp, correct_ray(obj, ray), &ints,
				obj);
		else if (obj->type == OBJ_PLANE)
			intersect_plane(&obj->shape.pl, correct_ray(obj, ray), &ints,
				obj);
		else if (obj->type == OBJ_CYLINDER)
			intersect_cylinder(&obj->shape.cy, correct_ray(obj, ray), &ints,
				obj);
		current = ints;
		if (inter_check(ints, current, max_distance))
			return (true);
		obj = obj->next;
	}
	return (free_intersections(ints), false);
}
