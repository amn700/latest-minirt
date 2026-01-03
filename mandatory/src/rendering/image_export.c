/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:25:56 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	write_pixel(FILE *output, mlx_image_t *img, int i)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = img->pixels[i + 0];
	g = img->pixels[i + 1];
	b = img->pixels[i + 2];
	fprintf(output, "%3u %3u %3u ", r, g, b);
	if ((i / 4 + 1) % img->width == 0)
		fprintf(output, "\n");
}

bool	mlx_image_to_ppm(mlx_image_t *img, char *string, unsigned int range)
{
	FILE	*output;
	int		i;
	int		total;

	output = fopen(string, "w");
	if (!output)
		return (false);
	fprintf(output, "P3\n");
	fprintf(output, "%i %i\n", img->width, img->height);
	fprintf(output, "%u\n", range);
	i = 0;
	total = (int)(img->width * img->height * 4);
	while (i < total)
	{
		write_pixel(output, img, i);
		i += 4;
	}
	fprintf(output, "\n");
	fclose(output);
	return (true);
}
