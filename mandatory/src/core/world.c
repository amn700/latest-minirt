/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_world	new_world(void)
{
	t_world	world;

	world.objects = NULL;
	world.lights = NULL;
	world.ambient_color = (t_tuple){1, 1, 1, 0};
	return (world);
}

t_object	*new_object(t_obj_type type, t_shapes shape)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = type;
	obj->shape = shape;
	obj->next = NULL;
	return (obj);
}

void	ft_add_object(t_object **lst, t_object *new)
{
	t_object	*ptr;

	if (!new || !lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}

t_light	*new_light(t_tuple point, t_tuple color)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->origin = point;
	light->color = color;
	light->brightness = 1.0;
	light->next = NULL;
	return (light);
}

void	ft_add_light(t_light **lst, t_light *new)
{
	t_light	*ptr;

	if (!new || !lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}
