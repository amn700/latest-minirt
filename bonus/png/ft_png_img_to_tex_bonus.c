/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_png_img_to_tex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_png_bonus.h"
#include <stdlib.h>

static t_color	**allocate_pixel_array(int width, int height)
{
	t_color	**pixels;
	int		y;

	pixels = (t_color **)malloc(sizeof(t_color *) * height);
	if (!pixels)
		return (NULL);
	y = 0;
	while (y < height)
	{
		pixels[y] = (t_color *)malloc(sizeof(t_color) * width);
		if (!pixels[y])
		{
			while (--y >= 0)
				free(pixels[y]);
			free(pixels);
			return (NULL);
		}
		y++;
	}
	return (pixels);
}

static void	convert_pixel(png_bytep row, int x, t_color *pixel, int channels)
{
	if (channels == 4)
	{
		pixel->r = row[x * 4];
		pixel->g = row[x * 4 + 1];
		pixel->b = row[x * 4 + 2];
		pixel->a = row[x * 4 + 3];
	}
	else if (channels == 3)
	{
		pixel->r = row[x * 3];
		pixel->g = row[x * 3 + 1];
		pixel->b = row[x * 3 + 2];
		pixel->a = 255;
	}
	else
	{
		pixel->r = row[x];
		pixel->g = row[x];
		pixel->b = row[x];
		pixel->a = 255;
	}
}

static int	get_channels(png_byte color_type)
{
	if (color_type == PNG_COLOR_TYPE_RGBA)
		return (4);
	else if (color_type == PNG_COLOR_TYPE_RGB)
		return (3);
	else
		return (1);
}

t_tex	ft_png_img_to_tex(t_png_img img)
{
	t_tex	tex;
	int		x;
	int		y;
	int		channels;

	tex.width = img.width;
	tex.height = img.height;
	tex.pixels = allocate_pixel_array(img.width, img.height);
	if (!tex.pixels)
	{
		tex.width = 0;
		tex.height = 0;
		return (tex);
	}
	channels = get_channels(img.color_type);
	y = 0;
	while (y < img.height)
	{
		x = 0;
		while (x < img.width)
		{
			convert_pixel(img.row_pointers[y], x, &tex.pixels[y][x], channels);
			x++;
		}
		y++;
	}
	return (tex);
}

void	ft_free_tex(t_tex *tex)
{
	int	y;

	if (!tex || !tex->pixels)
		return ;
	y = 0;
	while (y < tex->height)
	{
		if (tex->pixels[y])
			free(tex->pixels[y]);
		y++;
	}
	free(tex->pixels);
	tex->pixels = NULL;
	tex->width = 0;
	tex->height = 0;
}
