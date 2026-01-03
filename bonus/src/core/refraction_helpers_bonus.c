/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction_helpers_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

typedef struct s_container
{
	t_object			*obj;
	struct s_container	*next;
}	t_container;

static void	add_container(t_container **list, t_object *obj)
{
	t_container	*new;
	t_container	*tmp;

	new = malloc(sizeof(t_container));
	if (!new)
		return ;
	new->obj = obj;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static bool	remove_container(t_container **list, t_object *obj)
{
	t_container	*tmp;
	t_container	*prev;

	if (!*list)
		return (false);
	if ((*list)->obj == obj)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		return (true);
	}
	prev = *list;
	tmp = (*list)->next;
	while (tmp)
	{
		if (tmp->obj == obj)
		{
			prev->next = tmp->next;
			free(tmp);
			return (true);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (false);
}

static float	get_last_refractive_index(t_container *list)
{
	t_container	*tmp;
	float		index;

	if (!list)
		return (1.0);
	tmp = list;
	index = 1.0;
	while (tmp)
	{
		if (tmp->obj->type == OBJ_SPHERE)
			index = tmp->obj->shape.sp.material.refract_index;
		else if (tmp->obj->type == OBJ_PLANE)
			index = tmp->obj->shape.pl.material.refract_index;
		else if (tmp->obj->type == OBJ_CYLINDER)
			index = tmp->obj->shape.cy.material.refract_index;
		else if (tmp->obj->type == OBJ_CONE)
			index = tmp->obj->shape.co.material.refract_index;
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	return (index);
}

static void	free_containers(t_container *list)
{
	t_container	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

void	compute_n1_n2(t_inters *hit, t_inters *xs, t_comps *comps)
{
	t_container	*containers;
	t_inters	*current;

	containers = NULL;
	current = xs;
	while (current)
	{
		if (current == hit)
			comps->n1 = get_last_refractive_index(containers);
		if (remove_container(&containers, current->object))
			;
		else
			add_container(&containers, current->object);
		if (current == hit)
		{
			comps->n2 = get_last_refractive_index(containers);
			free_containers(containers);
			return ;
		}
		current = current->next;
	}
	free_containers(containers);
}
