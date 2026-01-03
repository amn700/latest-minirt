/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:45:49 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_matrix	inverse_matrix(t_matrix matrix)
{
	t_matrix	inverted;
	float		orginal_deter;
	int			row;
	int			col;

	inverted.size = 4;
	orginal_deter = matrix_determinant(matrix);
	row = 0;
	while (row < matrix.size)
	{
		col = 0;
		while (col < matrix.size)
		{
			inverted.mtrx[col][row] = compute_cofactor(matrix, row, col)
				/ orginal_deter;
			col++;
		}
		row++;
	}
	return (inverted);
}
