/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_png.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PNG_H
# define FT_PNG_H

# include "types.h"
# include <png.h>
# include <stdbool.h>

typedef struct s_png_img
{
	int				width;
	int				height;
	png_byte		color_type;
	png_byte		bit_depth;
	png_bytep		*row_pointers;
}	t_png_img;

/* PNG loading and conversion */
t_png_img	ft_allocate_png_img(const char *filename);
t_tex		ft_png_img_to_tex(t_png_img img);
bool		ft_read_png_file(const char *filename, t_png_img *img);
void		ft_free_png_img(t_png_img *img);

/* Texture management */
void		ft_free_tex(t_tex *tex);
bool		ft_write_png_file(const char *filename, t_tex tex);

#endif
