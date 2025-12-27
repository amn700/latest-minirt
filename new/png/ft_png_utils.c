/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_png_utils.c                                     :+:      :+:    :+:   */
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

static bool	init_png_write(FILE *fp, png_structp *png_ptr, png_infop *info_ptr)
{
	*png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!*png_ptr)
		return (false);
	*info_ptr = png_create_info_struct(*png_ptr);
	if (!*info_ptr)
	{
		png_destroy_write_struct(png_ptr, NULL);
		return (false);
	}
	if (setjmp(png_jmpbuf(*png_ptr)))
	{
		png_destroy_write_struct(png_ptr, info_ptr);
		return (false);
	}
	png_init_io(*png_ptr, fp);
	return (true);
}

static png_bytep	*create_row_pointers(t_tex tex)
{
	png_bytep	*row_pointers;
	int			y;
	int			x;

	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * tex.height);
	if (!row_pointers)
		return (NULL);
	y = 0;
	while (y < tex.height)
	{
		row_pointers[y] = (png_byte *)malloc(tex.width * 4);
		if (!row_pointers[y])
		{
			while (--y >= 0)
				free(row_pointers[y]);
			free(row_pointers);
			return (NULL);
		}
		x = 0;
		while (x < tex.width)
		{
			row_pointers[y][x * 4] = tex.pixels[y][x].r;
			row_pointers[y][x * 4 + 1] = tex.pixels[y][x].g;
			row_pointers[y][x * 4 + 2] = tex.pixels[y][x].b;
			row_pointers[y][x * 4 + 3] = tex.pixels[y][x].a;
			x++;
		}
		y++;
	}
	return (row_pointers);
}

static void	free_row_pointers(png_bytep *row_pointers, int height)
{
	int	y;

	if (!row_pointers)
		return ;
	y = 0;
	while (y < height)
	{
		if (row_pointers[y])
			free(row_pointers[y]);
		y++;
	}
	free(row_pointers);
}

bool	ft_write_png_file(const char *filename, t_tex tex)
{
	FILE			*fp;
	png_structp		png_ptr;
	png_infop		info_ptr;
	png_bytep		*row_pointers;

	fp = fopen(filename, "wb");
	if (!fp)
		return (false);
	if (!init_png_write(fp, &png_ptr, &info_ptr))
	{
		fclose(fp);
		return (false);
	}
	png_set_IHDR(png_ptr, info_ptr, tex.width, tex.height, 8,
		PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	row_pointers = create_row_pointers(tex);
	if (!row_pointers)
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return (false);
	}
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, NULL);
	free_row_pointers(row_pointers, tex.height);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return (true);
}
