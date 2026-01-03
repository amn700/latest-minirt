/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_submatrix.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:49:45 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	copy_row(float *dst, t_matrix matrix, int c_row, int col)
{
	int	c_col;
	int	n_col;

	c_col = 0;
	n_col = 0;
	while (c_col < matrix.size)
	{
		if (c_col != col)
		{
			dst[n_col] = matrix.mtrx[c_row][c_col];
			n_col++;
		}
		c_col++;
	}
}

t_matrix	get_sub_matrix(t_matrix matrix, int row, int col)
{
	t_matrix	new;
	int			c_row;
	int			n_row;

	new.size = matrix.size - 1;
	c_row = 0;
	n_row = 0;
	while (c_row < matrix.size)
	{
		if (c_row != row)
		{
			copy_row(new.mtrx[n_row], matrix, c_row, col);
			n_row++;
		}
		c_row++;
	}
	return (new);
}

float	get_matrix_minor_3x3(t_matrix matrix, int row, int col)
{
	t_matrix	sub;

	sub = get_sub_matrix(matrix, row, col);
	return (matrix_determinant_2x2(sub));
}

float	compute_cofactor(t_matrix matrix, int row, int col)
{
	t_matrix	sub;
	float		minor;

	sub = get_sub_matrix(matrix, row, col);
	minor = matrix_determinant(sub);
	if ((row + col) % 2 == 0)
		return (minor);
	return (-minor);
}
