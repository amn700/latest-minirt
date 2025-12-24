/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_tuple	position(t_ray ray, float distance)
{
	return (add_tuple(ray.origin, tuple_scalar_mult(ray.direction, distance)));
}

t_ray	transform_ray(t_ray ray, t_matrix m)
{
	t_tuple	new_origin;
	t_tuple	new_direction;

	new_origin = multiply_matrix_by_tuple(m, ray.origin);
	new_direction = multiply_matrix_by_tuple(m, ray.direction);
	return ((t_ray){new_origin, new_direction});
}
