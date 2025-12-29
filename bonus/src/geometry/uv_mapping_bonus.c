/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
 * Spherical UV mapping - shows equator (middle band) of sphere
 * Uses Y for latitude (vertical bands) and X/Z for longitude (horizontal wrap)
 * u = atan2(z, x) / (2*PI) + 0.5  (wraps around horizontally)
 * v = asin(y) / PI + 0.5 (bands from bottom to top)
 * Rotated by theta_offset to position features like 8-ball logo
 */
t_tuple	sphere_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	phi;
	float	u;
	float	v;
	float	theta_offset;

	theta_offset = 0.25f;  /* Rotate texture 90 degrees */
	theta = atan2f(local_point.z, local_point.x);  /* Swapped x,z for proper rotation */
	phi = asinf(local_point.y);  /* Use Y for vertical bands (equator at center) */
	u = theta / (2.0f * M_PI) + 0.5f + theta_offset;  /* Add offset rotation */
	v = phi / M_PI + 0.5f;
	
	/* Wrap u to [0, 1] range */
	if (u > 1.0f)
		u -= 1.0f;
	
	return ((t_tuple){u, v, 0, 0});
}

/*
 * Planar UV mapping - Linear tile-based mapping
 * Maps a point on a plane to UV coordinates with repeating texture tiles
 * Center of plane (0,0) shows center of texture
 * Texture repeats every 2 units in X and Z directions
 */
t_tuple	plane_uv_mapping(t_tuple local_point)
{
	float	u;
	float	v;
	float	scale;

	scale = 2.0f;  /* Texture repeats every 2 units. Adjust for larger/smaller tiles */
	
	/* Simple linear tiling: center of plane shows center of texture */
	u = fmodf(local_point.x / scale, 1.0f);
	v = fmodf(local_point.z / scale, 1.0f);
	
	/* Handle negative values */
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
 * theta_offset rotates texture so middle faces camera (negative Z direction)
 */
t_tuple	cylinder_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	u;
	float	v;
	float	theta_offset;

	theta_offset = 0.25f;
	theta = atan2f(local_point.x, local_point.z);
	u = theta / (2.0f * M_PI) + 0.5f + theta_offset;
	if (u > 1.0f)
		u -= 1.0f;
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
 * theta_offset rotates texture so middle faces camera (negative Z direction)
 */
t_tuple	cone_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	u;
	float	v;
	float	theta_offset;

	theta_offset = 0.25f;
	theta = atan2f(local_point.x, local_point.z);
	u = theta / (2.0f * M_PI) + 0.5f + theta_offset;
	if (u > 1.0f)
		u -= 1.0f;
	v = local_point.y - floorf(local_point.y);
	if (v < 0.0f)
		v += 1.0f;
	return ((t_tuple){u, v, 0, 0});
}
