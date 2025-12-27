/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_png_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_png.h"
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	cleanup_png(FILE *fp, png_structp png_ptr, png_infop info_ptr,
			png_bytep *row_pointers)
{
	int	i;

	if (row_pointers)
	{
		i = 0;
		while (row_pointers[i])
		{
			free(row_pointers[i]);
			i++;
		}
		free(row_pointers);
	}
	if (png_ptr)
		png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : NULL, NULL);
	if (fp)
		fclose(fp);
}

static bool	validate_png_header(FILE *fp)
{
	unsigned char	header[8];

	if (fread(header, 1, 8, fp) != 8)
		return (false);
	return (png_sig_cmp(header, 0, 8) == 0);
}

static bool	setup_png_read(FILE *fp, png_structp *png_ptr, png_infop *info_ptr)
{
	*png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!*png_ptr)
		return (false);
	*info_ptr = png_create_info_struct(*png_ptr);
	if (!*info_ptr)
	{
		png_destroy_read_struct(png_ptr, NULL, NULL);
		return (false);
	}
	if (setjmp(png_jmpbuf(*png_ptr)))
	{
		png_destroy_read_struct(png_ptr, info_ptr, NULL);
		return (false);
	}
	png_init_io(*png_ptr, fp);
	png_set_sig_bytes(*png_ptr, 8);
	return (true);
}

static bool	read_png_data(png_structp png_ptr, png_infop info_ptr,
			png_bytep **row_pointers, t_tex *out)
{
	int	y;

	png_read_info(png_ptr, info_ptr);
	out->width = png_get_image_width(png_ptr, info_ptr);
	out->height = png_get_image_height(png_ptr, info_ptr);
	png_set_expand(png_ptr);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_GRAY)
		png_set_gray_to_rgb(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	png_read_update_info(png_ptr, info_ptr);
	*row_pointers = malloc(sizeof(png_bytep) * (out->height + 1));
	if (!*row_pointers)
		return (false);
	y = 0;
	while (y < out->height)
	{
		(*row_pointers)[y] = malloc(png_get_rowbytes(png_ptr, info_ptr));
		if (!(*row_pointers)[y])
			return (false);
		y++;
	}
	(*row_pointers)[y] = NULL;
	png_read_image(png_ptr, *row_pointers);
	return (true);
}

static bool	convert_to_tex(png_bytep *row_pointers, t_tex *out)
{
	int	y;
	int	x;

	out->pixels = malloc(sizeof(t_color *) * out->height);
	if (!out->pixels)
		return (false);
	y = 0;
	while (y < out->height)
	{
		out->pixels[y] = malloc(sizeof(t_color) * out->width);
		if (!out->pixels[y])
		{
			while (--y >= 0)
				free(out->pixels[y]);
			free(out->pixels);
			return (false);
		}
		x = 0;
		while (x < out->width)
		{
			out->pixels[y][x].r = row_pointers[y][x * 4 + 0];
			out->pixels[y][x].g = row_pointers[y][x * 4 + 1];
			out->pixels[y][x].b = row_pointers[y][x * 4 + 2];
			out->pixels[y][x].a = row_pointers[y][x * 4 + 3];
			x++;
		}
		y++;
	}
	return (true);
}

bool	ft_load_png_to_tex(const char *path, t_tex *out)
{
	FILE			*fp;
	png_structp		png_ptr;
	png_infop		info_ptr;
	png_bytep		*row_pointers;

	if (!path || !out)
		return (false);
	fp = fopen(path, "rb");
	if (!fp)
		return (false);
	if (!validate_png_header(fp))
	{
		fclose(fp);
		return (false);
	}
	png_ptr = NULL;
	info_ptr = NULL;
	row_pointers = NULL;
	if (!setup_png_read(fp, &png_ptr, &info_ptr)
		|| !read_png_data(png_ptr, info_ptr, &row_pointers, out)
		|| !convert_to_tex(row_pointers, out))
	{
		cleanup_png(fp, png_ptr, info_ptr, row_pointers);
		return (false);
	}
	cleanup_png(fp, png_ptr, info_ptr, row_pointers);
	return (true);
}

void	ft_free_tex(t_tex *tex)
{
	int	i;

	if (!tex || !tex->pixels)
		return ;
	i = 0;
	while (i < tex->height)
	{
		if (tex->pixels[i])
			free(tex->pixels[i]);
		i++;
	}
	free(tex->pixels);
	tex->pixels = NULL;
	tex->width = 0;
	tex->height = 0;
}
