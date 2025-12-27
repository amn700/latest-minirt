/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/26 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
 * Spherical UV mapping
 * Maps a point on a sphere to UV coordinates [0,1]
 * u = atan2(x, z) / (2*PI) + 0.5
 * v = asin(y) / PI + 0.5
 */
t_tuple	sphere_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	phi;
	float	u;
	float	v;

	theta = atan2f(local_point.x, local_point.z);
	phi = asinf(local_point.y);
	u = theta / (2.0f * M_PI) + 0.5f;
	v = phi / M_PI + 0.5f;
	return ((t_tuple){u, v, 0, 0});
}

/*
 * Planar UV mapping
 * Maps a point on a plane to UV coordinates with tiling
 * Uses modulo for tiling effect
 */
t_tuple	plane_uv_mapping(t_tuple local_point)
{
	float	u;
	float	v;

	u = local_point.x - floorf(local_point.x);
	v = local_point.z - floorf(local_point.z);
	if (u < 0.0f)
		u += 1.0f;
	if (v < 0.0f)
		v += 1.0f;
	return ((t_tuple){u, v, 0, 0});
}

/*
 * Cylindrical UV mapping
 * u = atan2(x, z) / (2*PI) + 0.5 (around circumference)
 * v = y (along height, modulo for tiling)
 */
t_tuple	cylinder_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	u;
	float	v;

	theta = atan2f(local_point.x, local_point.z);
	u = theta / (2.0f * M_PI) + 0.5f;
	v = local_point.y - floorf(local_point.y);
	if (v < 0.0f)
		v += 1.0f;
	return ((t_tuple){u, v, 0, 0});
}

/*
 * Conical UV mapping
 * Similar to cylinder but adjusted for cone shape
 * u = atan2(x, z) / (2*PI) + 0.5
 * v = y (along height)
 */
t_tuple	cone_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	u;
	float	v;

	theta = atan2f(local_point.x, local_point.z);
	u = theta / (2.0f * M_PI) + 0.5f;
	v = local_point.y - floorf(local_point.y);
	if (v < 0.0f)
		v += 1.0f;
	return ((t_tuple){u, v, 0, 0});
}
