/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/types.h"
#include "../../includes/math_ops.h"
#include <math.h>

// UV mapping for plane (planar projection)
static t_tuple	ft_plane_uv_mapping(t_tuple local_point)
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

// UV mapping for sphere (spherical coordinates)
static t_tuple	ft_sphere_uv_mapping(t_tuple local_point)
{
	float	theta;
	float	phi;
	float	u;
	float	v;

	theta = atan2f(local_point.x, local_point.z);
	phi = acosf(local_point.y);
	u = 1.0f - ((theta / (2.0f * M_PI)) + 0.5f);
	v = phi / M_PI;
	return ((t_tuple){u, v, 0, 0});
}

// UV mapping for cylinder (cylindrical coordinates)
static t_tuple	ft_cylinder_uv_mapping(t_tuple local_point)
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

// Get UV coordinates based on object type
t_tuple	ft_get_uv_coords(t_tuple local_point, t_obj_type obj_type)
{
	if (obj_type == OBJ_SPHERE)
		return (ft_sphere_uv_mapping(local_point));
	else if (obj_type == OBJ_PLANE)
		return (ft_plane_uv_mapping(local_point));
	else if (obj_type == OBJ_CYLINDER)
		return (ft_cylinder_uv_mapping(local_point));
	return ((t_tuple){0, 0, 0, 0});
}

// Sample texture at UV coordinates
t_tuple	ft_uv_pattern_at(t_tex texture, float u, float v)
{
	int		x;
	int		y;
	t_color	color;

	if (!texture.pixels || texture.width == 0 || texture.height == 0)
		return ((t_tuple){1, 1, 1, 1});
	u = u - floorf(u);
	v = v - floorf(v);
	if (u < 0.0f)
		u += 1.0f;
	if (v < 0.0f)
		v += 1.0f;
	x = (int)(u * (texture.width - 1));
	y = (int)(v * (texture.height - 1));
	if (x >= texture.width)
		x = texture.width - 1;
	if (y >= texture.height)
		y = texture.height - 1;
	color = texture.pixels[y][x];
	return ((t_tuple){color.r / 255.0f, color.g / 255.0f,
		color.b / 255.0f, 1.0f});
}
