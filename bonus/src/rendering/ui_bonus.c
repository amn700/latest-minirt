/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	fill_rect(mlx_image_t *img, int x, int y, int w, int h, uint32_t c)
{
	int	i;
	int	j;

	j = 0;
	while (j < h && (y + j) < (int)img->height)
	{
		i = 0;
		while (i < w && (x + i) < (int)img->width)
		{
			if (x + i >= 0 && y + j >= 0)
				mlx_put_pixel(img, x + i, y + j, c);
			i++;
		}
		j++;
	}
}

void	draw_front_page(t_data *data)
{
	int				w;
	int				h;
	mlx_texture_t	*tex;

	w = (int)data->cam.h_view;
	h = (int)data->cam.v_view;
	tex = mlx_load_png("front1.png");
	if (tex)
	{
		data->front = mlx_texture_to_image(data->ptr, tex);
		mlx_delete_texture(tex);
		if (data->front)
		{
			mlx_resize_image(data->front, w, h);
			mlx_image_to_window(data->ptr, data->front, 0, 0);
			return ;
		}
	}
	data->front = mlx_new_image(data->ptr, w, h);
	if (!data->front)
		return ;
	fill_rect(data->front, 0, 0, w, h, 0x1a1a2eFF);
	mlx_image_to_window(data->ptr, data->front, 0, 0);
}

void	draw_progress_bar(t_data *data)
{
	int	w;
	int	h;
	int	bar_w;
	int	bar_h;
	int	bar_x;
	int	bar_y;

	w = (int)data->cam.h_view;
	h = (int)data->cam.v_view;
	bar_w = 600;
	bar_h = 30;
	bar_x = (w - bar_w) / 2;
	bar_y = h - 80;
	data->bar_bg = mlx_new_image(data->ptr, bar_w, bar_h);
	data->bar_fill = mlx_new_image(data->ptr, bar_w - 4, bar_h - 4);
	if (!data->bar_bg || !data->bar_fill)
		return ;
	fill_rect(data->bar_bg, 0, 0, bar_w, bar_h, 0x333333FF);
	fill_rect(data->bar_fill, 0, 0, 1, bar_h - 4, 0x4CAF50FF);
	mlx_image_to_window(data->ptr, data->bar_bg, bar_x, bar_y);
	mlx_image_to_window(data->ptr, data->bar_fill, bar_x + 2, bar_y + 2);
}

static void	set_image_color(mlx_image_t *img, uint32_t color)
{
	uint32_t	i;
	uint8_t		*pixel;

	if (!img)
		return ;
	i = 0;
	while (i < img->width * img->height)
	{
		pixel = &img->pixels[i * 4];
		if (pixel[3] > 0)
		{
			pixel[0] = (color >> 24) & 0xFF;
			pixel[1] = (color >> 16) & 0xFF;
			pixel[2] = (color >> 8) & 0xFF;
		}
		i++;
	}
}

static void	draw_done_message(t_data *data)
{
	int	w;
	int	h;
	int	msg_x;
	int	msg_y;

	if (data->bar_text)
		return ;
	w = (int)data->cam.h_view;
	h = (int)data->cam.v_view;
	msg_x = (w / 2) - 120;
	msg_y = h - 130;
	data->bar_text = mlx_put_string(data->ptr, "Done! Press SPACE to view", msg_x, msg_y);
	set_image_color(data->bar_text, 0x000000FF);
}

void	update_progress_bar(t_data *data)
{
	float		progress;
	int			fill_width;
	int			bar_w;
	int			bar_h;
	int			x;
	int			y;
	uint32_t	color;

	if (!data->bar_fill || data->total_lines == 0)
		return ;
	bar_w = 596;
	bar_h = 26;
	progress = (float)data->lines_done / (float)data->total_lines;
	if (progress > 1.0f)
		progress = 1.0f;
	fill_width = (int)(bar_w * progress);
	y = 0;
	while (y < bar_h)
	{
		x = 0;
		while (x < bar_w)
		{
			if (x < fill_width)
				color = 0x4CAF50FF;
			else
				color = 0x222222FF;
			mlx_put_pixel(data->bar_fill, x, y, color);
			x++;
		}
		y++;
	}
	if (data->lines_done >= data->total_lines && !data->render_complete)
	{
		data->render_complete = true;
		data->state = STATE_WAITING;
		draw_done_message(data);
	}
}

void	hide_ui_elements(t_data *data)
{
	if (data->front)
		data->front->enabled = false;
	if (data->bar_bg)
		data->bar_bg->enabled = false;
	if (data->bar_fill)
		data->bar_fill->enabled = false;
	if (data->bar_text)
		data->bar_text->enabled = false;
}

void	show_rendered_image(t_data *data)
{
	hide_ui_elements(data);
	if (data->img)
		data->img->enabled = true;
	data->state = STATE_DISPLAY;
}
