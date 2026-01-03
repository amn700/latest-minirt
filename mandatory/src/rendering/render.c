/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:54:58 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void		print_progress_bar(int current, int total, int bar_width);
uint32_t	tuple_to_pixel(t_tuple color);

static void	render_pixel(mlx_image_t *img, t_camera cam,
		t_world world, int *coords)
{
	t_ray		ray;
	t_tuple		color;
	uint32_t	pixel_color;

	ray = ray_for_pixel(cam, (double)coords[0], (double)coords[1]);
	color = color_at(world, ray);
	pixel_color = tuple_to_pixel(color);
	mlx_put_pixel(img, coords[0], coords[1], pixel_color);
}

static void	render_loop(mlx_image_t *img, t_camera cam,
		t_world world, int *coords)
{
	coords[1] = 0;
	while (coords[1] < (int)cam.v_view)
	{
		coords[0] = 0;
		while (coords[0] < (int)cam.h_view)
		{
			render_pixel(img, cam, world, coords);
			coords[3]++;
			if ((coords[3] * 100) / coords[2]
				!= ((coords[3] - 1) * 100) / coords[2])
				print_progress_bar(coords[3], coords[2], 50);
			coords[0]++;
		}
		coords[1]++;
	}
}

mlx_image_t	*render(mlx_t *mlx, t_camera cam, t_world world)
{
	mlx_image_t	*img;
	int			coords[4];

	if (!mlx)
		return (NULL);
	img = mlx_new_image(mlx, (int)cam.h_view, (int)cam.v_view);
	if (!img)
		return (NULL);
	coords[2] = (int)cam.h_view * (int)cam.v_view;
	coords[3] = 0;
	write(1, "\n", 1);
	render_loop(img, cam, world, coords);
	write(1, "\n", 1);
	return (img);
}
