/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_parsers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 09:01:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

float	ft_atof(char *str)
{
	float	result;
	float	sign;
	float	decimal_place;

	result = 0.0f;
	sign = 1.0f;
	decimal_place = 0.1f;
	if (*str == '-')
	{
		sign = -1.0f;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		result = result * 10.0f + (*str++ - '0');
	if (*str++ == '.')
	{
		while (*str >= '0' && *str <= '9')
		{
			result += (*str++ - '0') * decimal_place;
			decimal_place *= 0.1f;
		}
	}
	return (result * sign);
}

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
			if (!i_field_validation(fields[i]) || \
				!i_range_validator(min, max, fields[i]))
				return (false);
			i++;
		}
	}
	else
	{
		while (i < 3)
		{
			if (!f_field_validation(fields[i]) || \
				!f_range_validator(min, max, fields[i]))
				return (false);
			i++;
		}
	}
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
