/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transformations_basic.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	init_identity_matrix(t_matrix *m)
{
	int	row;
	int	col;

	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			m->mtrx[row][col] = 0;
			if (col == row)
				m->mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
}

t_matrix	translation(float x, float y, float z)
{
	t_matrix	new;

	init_identity_matrix(&new);
	new.mtrx[0][3] = x;
	new.mtrx[1][3] = y;
	new.mtrx[2][3] = z;
	new.size = 4;
	return (new);
}

t_matrix	scaling(float x, float y, float z)
{
	t_matrix	new;

	init_identity_matrix(&new);
	new.mtrx[0][0] = x;
	new.mtrx[1][1] = y;
	new.mtrx[2][2] = z;
	new.size = 4;
	return (new);
}

double	to_radians(int degrees)
{
	return (degrees * M_PI / 180.0);
}
