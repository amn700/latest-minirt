/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_camera	camera(double hsize, double vsize, double field_of_view)
{
	t_camera	cam;
	double		half_view;
	double		aspect;

	cam.h_view = hsize;
	cam.v_view = vsize;
	cam.fov = field_of_view;
	cam.transform = identity();
	cam.transform.size = 4;
	half_view = tan(field_of_view / 2);
	aspect = hsize / vsize;
	if (aspect >= 1)
	{
		cam.half_width = half_view;
		cam.half_height = half_view / aspect;
	}
	else
	{
		cam.half_width = half_view * aspect;
		cam.half_height = half_view;
	}
	cam.pixel_size = (cam.half_width * 2) / cam.h_view;
	return (cam);
}

t_ray	ray_for_pixel(t_camera cam, double px, double py)
{
	double		xoffset;
	double		yoffset;
	double		world_x;
	double		world_y;
	t_matrix	inv;
	t_tuple		pixel;
	t_tuple		origin;
	t_tuple		direction;

	xoffset = (px + 0.5) * cam.pixel_size;
	yoffset = (py + 0.5) * cam.pixel_size;
	world_x = cam.half_width - xoffset;
	world_y = cam.half_height - yoffset;
	inv = inverse_matrix(cam.transform);
	pixel = multiply_matrix_by_tuple(inv, (t_tuple){world_x, world_y, -1, 1});
	origin = multiply_matrix_by_tuple(inv, (t_tuple){0, 0, 0, 1});
	direction = normalizing_vector(substract_tuple(pixel, origin));
	return ((t_ray){origin, direction});
}
