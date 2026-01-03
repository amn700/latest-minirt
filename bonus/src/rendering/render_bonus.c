/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

uint32_t	tuple_to_pixel(t_tuple color)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint32_t	pixel_color;

	r = (uint8_t)(fmin(fmax(color.x, 0.0), 1.0) * 255);
	g = (uint8_t)(fmin(fmax(color.y, 0.0), 1.0) * 255);
	b = (uint8_t)(fmin(fmax(color.z, 0.0), 1.0) * 255);
	pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
	return (pixel_color);
}

static void	render_line(t_render_ctx *ctx, int y)
{
	int			x;
	t_ray		ray;
	t_tuple		color;
	uint32_t	pixel_color;

	x = 0;
	while (x < (int)ctx->cam.h_view)
	{
		if (*(ctx->stop_flag))
			return ;
		ray = ray_for_pixel(ctx->cam, (double)x, (double)y);
		color = color_at(ctx->world, ray, MAX_REFLECTION_DEPTH);
		pixel_color = tuple_to_pixel(color);
		mlx_put_pixel(ctx->img, x, y, pixel_color);
		x++;
	}
}

void	*render_thread_func(void *arg)
{
	t_render_ctx	*ctx;
	int				y;

	ctx = (t_render_ctx *)arg;
	y = ctx->start_y;
	while (y < ctx->end_y)
	{
		if (*(ctx->stop_flag))
			break ;
		render_line(ctx, y);
		pthread_mutex_lock(ctx->mutex);
		(*(ctx->lines_done))++;
		pthread_mutex_unlock(ctx->mutex);
		y++;
	}
	return (NULL);
}

void	start_threaded_render(t_data *data)
{
	int		i;
	int		lines_per_thread;
	int		start_y;

	data->lines_done = 0;
	data->total_lines = (int)data->cam.v_view;
	data->stop_flag = 0;
	data->render_complete = false;
	data->threads_started = false;
	data->mutex_initialized = false;
	pthread_mutex_init(&data->render_mutex, NULL);
	data->mutex_initialized = true;
	lines_per_thread = data->total_lines / NUM_THREADS;
	start_y = 0;
	i = 0;
	while (i < NUM_THREADS)
	{
		data->thread_ctx[i].img = data->img;
		data->thread_ctx[i].cam = data->cam;
		data->thread_ctx[i].world = data->world;
		data->thread_ctx[i].start_y = start_y;
		if (i == NUM_THREADS - 1)
			data->thread_ctx[i].end_y = data->total_lines;
		else
			data->thread_ctx[i].end_y = start_y + lines_per_thread;
		data->thread_ctx[i].lines_done = &data->lines_done;
		data->thread_ctx[i].mutex = &data->render_mutex;
		data->thread_ctx[i].stop_flag = &data->stop_flag;
		pthread_create(&data->threads[i], NULL, render_thread_func,
			&data->thread_ctx[i]);
		start_y += lines_per_thread;
		i++;
	}
	data->threads_started = true;
}

void	stop_threaded_render(t_data *data)
{
	int	i;

	data->stop_flag = 1;
	if (data->threads_started)
	{
		i = 0;
		while (i < NUM_THREADS)
		{
			pthread_join(data->threads[i], NULL);
			i++;
		}
		data->threads_started = false;
	}
	if (data->mutex_initialized)
	{
		pthread_mutex_destroy(&data->render_mutex);
		data->mutex_initialized = false;
	}
}

mlx_image_t	*render(mlx_t *mlx, t_camera cam, t_world world)
{
	mlx_image_t	*img;
	int			x;
	int			y;
	t_ray		ray;
	t_tuple		color;

	if (!mlx)
		return (NULL);
	img = mlx_new_image(mlx, (int)cam.h_view, (int)cam.v_view);
	if (!img)
		return (NULL);
	y = 0;
	while (y < (int)cam.v_view)
	{
		x = 0;
		while (x < (int)cam.h_view)
		{
			ray = ray_for_pixel(cam, (double)x, (double)y);
			color = color_at(world, ray, MAX_REFLECTION_DEPTH);
			mlx_put_pixel(img, x, y, tuple_to_pixel(color));
			x++;
		}
		y++;
	}
	return (img);
}
