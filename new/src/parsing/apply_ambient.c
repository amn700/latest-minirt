/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/01 11:58:23 by amn              ###   ########.fr       */
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
