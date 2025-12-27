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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool	init_png_read(FILE *fp, png_structp *png_ptr, png_infop *info_ptr)
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
	return (true);
}

static bool	allocate_row_pointers(t_png_img *img)
{
	int	y;

	img->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * img->height);
	if (!img->row_pointers)
		return (false);
	y = 0;
	while (y < img->height)
	{
		img->row_pointers[y] = NULL;
		y++;
	}
	return (true);
}

bool	ft_read_png_file(const char *filename, t_png_img *img)
{
	FILE			*fp;
	png_structp		png_ptr;
	png_infop		info_ptr;

	fp = fopen(filename, "rb");
	if (!fp)
		return (false);
	if (!init_png_read(fp, &png_ptr, &info_ptr))
	{
		fclose(fp);
		return (false);
	}
	png_read_info(png_ptr, info_ptr);
	img->width = png_get_image_width(png_ptr, info_ptr);
	img->height = png_get_image_height(png_ptr, info_ptr);
	img->color_type = png_get_color_type(png_ptr, info_ptr);
	img->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	if (!allocate_row_pointers(img))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return (false);
	}
	png_read_image(png_ptr, img->row_pointers);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	return (true);
}

t_png_img	ft_allocate_png_img(const char *filename)
{
	t_png_img	img;

	img.width = 0;
	img.height = 0;
	img.color_type = 0;
	img.bit_depth = 0;
	img.row_pointers = NULL;
	if (!ft_read_png_file(filename, &img))
	{
		img.width = 0;
		img.height = 0;
	}
	return (img);
}

void	ft_free_png_img(t_png_img *img)
{
	int	y;

	if (!img || !img->row_pointers)
		return ;
	y = 0;
	while (y < img->height)
	{
		if (img->row_pointers[y])
			free(img->row_pointers[y]);
		y++;
	}
	free(img->row_pointers);
	img->row_pointers = NULL;
	img->width = 0;
	img->height = 0;
}
