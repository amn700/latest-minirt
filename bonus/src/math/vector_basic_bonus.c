/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_basic_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/15 18:36:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

float	vector_magnitude(t_tuple v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

t_tuple	normalizing_vector(t_tuple v)
{
	float	mag;

	mag = vector_magnitude(v);
	if (mag == 0)
		return (v);
	return ((t_tuple){v.x / mag, v.y / mag, v.z / mag, 0});
}

float	vecs_dot_product(t_tuple va, t_tuple vb)
{
	return (va.x * vb.x + va.y * vb.y + va.z * vb.z + va.w * vb.w);
}
