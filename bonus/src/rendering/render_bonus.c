/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/14 23:52:24 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	print_progress_bar(int current, int total, int bar_width)
{
	float	progress;
	int		pos;
	int		i;
	char	percent_buf[8];
	int		pct;

	progress = total > 0 ? (float)current / (float)total : 0.0f;
	if (progress < 0.0f)
		progress = 0.0f;
	if (progress > 1.0f)
		progress = 1.0f;
	pos = (int)(bar_width * progress);
	write(1, "\r[", 2);
	i = 0;
	while (i < bar_width)
	{
		if (i < pos)
			write(1, "#", 1);
		else if (i == pos && pos < bar_width)
			write(1, ">", 1);
		else
			write(1, " ", 1);
		i++;
	}
	write(1, "] ", 2);
	pct = (int)(progress * 100.0f);
	if (pct < 0)
		pct = 0;
	if (pct > 100)
		pct = 100;
	int n = snprintf(percent_buf, sizeof(percent_buf), "%d%%", pct);
	if (n > 0)
		write(1, percent_buf, (size_t)n);
	if (current == total)
		write(1, "\n", 1);
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
	color = color_at(world, ray, MAX_REFLECTION_DEPTH);
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
	printf("\r[                                                  ] 0%%");
	fflush(stdout);
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
