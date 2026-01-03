/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	write_char(char c)
{
	write(1, &c, 1);
}

static void	write_number(int n)
{
	char	c;

	if (n >= 10)
		write_number(n / 10);
	c = '0' + (n % 10);
	write(1, &c, 1);
}

void	print_progress_bar(int current, int total, int bar_width)
{
	float	progress;
	int		pos;
	int		i;

	progress = (float)current / total;
	pos = bar_width * progress;
	write_char('\r');
	write_char('[');
	i = 0;
	while (i < bar_width)
	{
		if (i < pos)
			write_char('#');
		else if (i == pos)
			write_char('>');
		else
			write_char(' ');
		i++;
	}
	write(1, "] ", 2);
	write_number((int)(progress * 100));
	write_char('%');
	if (current == total)
		write_char('\n');
}

uint32_t	tuple_to_pixel(t_tuple color)
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
