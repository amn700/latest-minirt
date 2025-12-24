/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transformations_rotation.c                         :+:      :+:    :+:   */
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

t_matrix	rotation_x(float theta)
{
	t_matrix	new;

	new.size = 4;
	init_identity_matrix(&new);
	new.mtrx[1][1] = cos(theta);
	new.mtrx[1][2] = -sin(theta);
	new.mtrx[2][1] = sin(theta);
	new.mtrx[2][2] = cos(theta);
	return (new);
}

t_matrix	rotation_y(float theta)
{
	t_matrix	new;

	new.size = 4;
	init_identity_matrix(&new);
	new.mtrx[0][0] = cos(theta);
	new.mtrx[0][2] = sin(theta);
	new.mtrx[2][0] = -sin(theta);
	new.mtrx[2][2] = cos(theta);
	return (new);
}

t_matrix	rotation_z(float theta)
{
	t_matrix	new;

	new.size = 4;
	init_identity_matrix(&new);
	new.mtrx[0][0] = cos(theta);
	new.mtrx[0][1] = -sin(theta);
	new.mtrx[1][0] = sin(theta);
	new.mtrx[1][1] = cos(theta);
	return (new);
}
