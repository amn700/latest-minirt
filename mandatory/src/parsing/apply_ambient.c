/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 12:00:09 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * Applies the ambient light ratio from the parsed data to all objects
 * This ensures all objects use the scene's ambient lighting configuration
 */
void	apply_ambient_to_objects(t_data *data)
{
	t_object	*current;
	float		ambient_ratio;

	if (!data || !data->object)
		return ;
	ambient_ratio = data->ambl.l_ratio;
	current = data->object;
	while (current)
	{
		if (current->type == OBJ_SPHERE)
			current->shape.sp.material.ambient = ambient_ratio;
		else if (current->type == OBJ_PLANE)
			current->shape.pl.material.ambient = ambient_ratio;
		else if (current->type == OBJ_CYLINDER)
			current->shape.cy.material.ambient = ambient_ratio;
		current = current->next;
	}
}

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
