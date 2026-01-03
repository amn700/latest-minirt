/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:47:25 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

bool	i_field_validation(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

bool	f_validator_core(char *str, int *i, int *dot_count, int *digit_count)
{
	while (str[*i])
	{
		if (str[*i] >= '0' && str[*i] <= '9')
		{
			(*digit_count)++;
			(*i)++;
		}
		else if (str[*i] == '.')
		{
			(*dot_count)++;
			if (*dot_count > 1)
				return (false);
			(*i)++;
		}
		else
			return (false);
	}
	return (true);
}

bool	f_field_validation(char *str)
{
	int		i;
	int		dot_count;
	int		digit_count;

	if (!str || !str[0])
		return (false);
	i = 0;
	dot_count = 0;
	digit_count = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!f_validator_core(str, &i, &dot_count, &digit_count))
		return (false);
	if (digit_count == 0)
		return (false);
	return (true);
}

bool	i_range_validator(int min, int max, char *line)
{
	int	s;

	s = ft_atoi(line);
	if (s < min || s > max)
		return (false);
	return (true);
}

bool	f_range_validator(float min, float max, char *line)
{
	float	f;

	f = ft_atof(line);
	if (f < min || f > max)
		return (false);
	return (true);
}
