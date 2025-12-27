/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include <math.h>

/*
 * Sphere UV mapping using spherical coordinates
 * u: longitude (atan2(x, z)), v: latitude (acos(y))
 */
void	sphere_uv(t_tuple point, float *u, float *v)
{
	float	theta;
	float	phi;
	float	raw_u;

	theta = atan2(point.x, point.z);
	phi = acos(point.y);
	raw_u = theta / (2.0f * M_PI);
	*u = 1.0f - (raw_u + 0.5f);
	*v = phi / M_PI;
	if (*u < 0.0f)
		*u += 1.0f;
	if (*u > 1.0f)
		*u -= 1.0f;
	if (*v < 0.0f)
		*v = 0.0f;
	if (*v > 1.0f)
		*v = 1.0f;
}

/*
 * Plane UV mapping using planar coordinates
 * u: x coordinate with wrapping, v: z coordinate with wrapping
 */
void	plane_uv(t_tuple point, float *u, float *v)
{
	*u = point.x - floor(point.x);
	*v = point.z - floor(point.z);
	if (*u < 0.0f)
		*u += 1.0f;
	if (*v < 0.0f)
		*v += 1.0f;
}

/*
 * Cylinder UV mapping using cylindrical coordinates
 * u: circumference (atan2(x, z)), v: height (y with wrapping)
 */
void	cylinder_uv(t_tuple point, float *u, float *v)
{
	float	theta;
	float	raw_u;

	theta = atan2(point.x, point.z);
	raw_u = theta / (2.0f * M_PI);
	*u = 1.0f - (raw_u + 0.5f);
	*v = point.y - floor(point.y);
	if (*u < 0.0f)
		*u += 1.0f;
	if (*u > 1.0f)
		*u -= 1.0f;
	if (*v < 0.0f)
		*v += 1.0f;
}
