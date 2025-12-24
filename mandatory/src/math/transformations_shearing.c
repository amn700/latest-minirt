/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transformations_shearing.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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

t_matrix	shearing(t_propo propotion)
{
	t_matrix	new;

	new.size = 4;
	init_identity_matrix(&new);
	new.mtrx[0][1] = propotion.xy;
	new.mtrx[0][2] = propotion.xz;
	new.mtrx[1][0] = propotion.yx;
	new.mtrx[1][2] = propotion.yz;
	new.mtrx[2][0] = propotion.zx;
	new.mtrx[2][1] = propotion.zy;
	return (new);
}
