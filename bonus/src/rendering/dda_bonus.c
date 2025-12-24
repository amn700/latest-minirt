/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:25:06 by mohchaib          #+#    #+#             */
/*   Updated: 2025/09/30 12:50:06 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

float	ft_step(t_screen *d)
{
	if (fabs(d->x) > fabs(d->y))
		return (fabs(d->x));
	return (fabs(d->y));
}

void	draw_line(t_screen one, t_screen two, mlx_image_t *img, int color)
{
	t_screen	screen;
	t_screen	d;
	float		step;
	float		i;

	screen.x = one.x;
	screen.y = one.y;
	d.x = two.x - one.x;
	d.y = two.y - one.y;
	step = ft_step(&d);
	d.x /= step;
	d.y /= step;
	i = 0;
	while (i <= step)
	{
		if (screen.x >= 0 && screen.y >= 0 && screen.x < img->width
			&& screen.y < img->height)
			mlx_put_pixel(img, (int)screen.x, (int)screen.y, color);
		screen.x += d.x;
		screen.y += d.y;
		i++;
	}
}
