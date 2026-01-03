/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:25:10 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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
	t_matrix	inv;
	t_tuple		pixel;
	t_tuple		origin;
	t_tuple		direction;

	inv = inverse_matrix(cam.transform);
	pixel = multiply_matrix_by_tuple(inv, (t_tuple){cam.half_width - (px + 0.5)
			* cam.pixel_size, cam.half_height - (py + 0.5) * cam.pixel_size, -1,
			1});
	origin = multiply_matrix_by_tuple(inv, (t_tuple){0, 0, 0, 1});
	direction = normalizing_vector(substract_tuple(pixel, origin));
	return ((t_ray){origin, direction});
}
