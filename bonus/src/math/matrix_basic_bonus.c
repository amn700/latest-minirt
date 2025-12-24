/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_matrix	identity(void)
{
	t_matrix	identity_matrix;
	int			row;
	int			col;

	identity_matrix.size = 4;
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			identity_matrix.mtrx[row][col] = 0;
			if (col == row)
				identity_matrix.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	return (identity_matrix);
}

bool	compare_matrix(t_matrix a, t_matrix b)
{
	int	i;
	int	j;

	i = 0;
	while (i < a.size)
	{
		j = 0;
		while (j < b.size)
		{
			if (!f_equal(a.mtrx[i][j], b.mtrx[i][j]))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

t_matrix	transposing_matrix(t_matrix matrix)
{
	t_matrix	new;
	int			i;
	int			j;

	new.size = matrix.size;
	i = 0;
	while (i < matrix.size)
	{
		j = 0;
		while (j < matrix.size)
		{
			new.mtrx[i][j] = matrix.mtrx[j][i];
			j++;
		}
		i++;
	}
	return (new);
}
