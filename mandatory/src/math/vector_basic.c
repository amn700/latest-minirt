/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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
	return ((t_tuple){v.x / mag, v.y / mag, v.z / mag, v.w / mag});
}

float	vecs_dot_product(t_tuple va, t_tuple vb)
{
	return (va.x * vb.x + va.y * vb.y + va.z * vb.z + va.w * vb.w);
}
