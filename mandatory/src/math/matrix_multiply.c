/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_multiply.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:45:49 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_matrix	multiply_matrix(t_matrix a, t_matrix b)
{
	t_matrix	new;
	int			i;
	int			j;
	int			k;

	new.size = a.size;
	i = 0;
	while (i < a.size)
	{
		j = 0;
		while (j < b.size)
		{
			new.mtrx[i][j] = 0;
			k = 0;
			while (k < a.size)
			{
				new.mtrx[i][j] += a.mtrx[i][k] * b.mtrx[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (new);
}

t_tuple	multiply_matrix_by_tuple(t_matrix m, t_tuple v)
{
	t_matrix	new;

	new.mtrx[0][0] = v.x;
	new.mtrx[1][0] = v.y;
	new.mtrx[2][0] = v.z;
	new.mtrx[3][0] = v.w;
	new.size = 4;
	new = multiply_matrix(m, new);
	return ((t_tuple){new.mtrx[0][0], new.mtrx[1][0],
			new.mtrx[2][0], new.mtrx[3][0]});
}
