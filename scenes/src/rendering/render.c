/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	print_progress_bar(int current, int total, int bar_width)
{
	float	progress;
	int		pos;
	int		i;

	progress = (float)current / total;
	pos = bar_width * progress;
	printf("\r[");
	i = 0;
	while (i < bar_width)
	{
		if (i < pos)
			printf("█");
		else if (i == pos)
			printf(">");
		else
			printf(" ");
		i++;
	}
	printf("] %d%%", (int)(progress * 100));
	fflush(stdout);
	if (current == total)
		printf("\n");
}

static uint32_t	tuple_to_pixel(t_tuple color)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint32_t	pixel_color;

	r = (uint8_t)(fmin(color.x, 1.0) * 255);
	g = (uint8_t)(fmin(color.y, 1.0) * 255);
	b = (uint8_t)(fmin(color.z, 1.0) * 255);
	pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
	return (pixel_color);
}

static void	render_pixel(mlx_image_t *img, t_camera cam, \
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
	printf("\n🎨 Rendering %dx%d scene...\n", (int)cam.h_view, \
		(int)cam.v_view);
	coords[1] = 0;
	while (coords[1] < (int)cam.v_view)
	{
		coords[0] = 0;
		while (coords[0] < (int)cam.h_view)
		{
			render_pixel(img, cam, world, coords);
			coords[3]++;
			if ((coords[3] * 100) / coords[2] != ((coords[3] - 1) * 100) / coords[2])
				print_progress_bar(coords[3], coords[2], 50);
			coords[0]++;
		}
		coords[1]++;
	}
	printf("✅ Rendering complete!\n\n");
	return (img);
}
