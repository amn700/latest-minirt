/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_determinant.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

float	matrix_determinant_2x2(t_matrix matrix)
{
	return ((float)(matrix.mtrx[0][0] * matrix.mtrx[1][1]) - \
		(matrix.mtrx[1][0] * matrix.mtrx[0][1]));
}

float	matrix_determinant(t_matrix matrix)
{
	float	deter;
	int		col;
	float	cofactor;

	deter = 0;
	if (matrix.size == 2)
		deter = matrix_determinant_2x2(matrix);
	else
	{
		col = 0;
		while (col < matrix.size)
		{
			cofactor = compute_cofactor(matrix, 0, col);
			deter = deter + (matrix.mtrx[0][col] * cofactor);
			col++;
		}
	}
	return (deter);
}

bool	check_if_invertible(t_matrix matrix)
{
	return (matrix_determinant(matrix) != 0);
}
