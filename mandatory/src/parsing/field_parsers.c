/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_parsers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:59:35 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	skip_spaces(char *line, int *i)
{
	while (line[*i] == 32 || (line[*i] >= 9 && line[*i] <= 13))
		(*i)++;
}

bool	check_fields_num(char **fields, int n)
{
	int	i;

	i = 0;
	while (fields[i])
		i++;
	if (i != n)
		return (false);
	return (true);
}

bool	float_val_core(char **fields, float min, float max)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (!f_field_validation(fields[i])
			|| !f_range_validator(min, max, fields[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	tuple_validator(char **fields, bool b, float min, float max)
{
	int	i;

	if (!fields || !fields[0] || !fields[1] || !fields[2])
		return (false);
	i = 0;
	if (b)
	{
		while (i < 3)
		{
			if (!i_field_validation(fields[i])
				|| !i_range_validator(min, max, fields[i]))
				return (false);
			i++;
		}
	}
	else
		if (!float_val_core(fields, min, max))
			return (false);
	return (true);
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}
