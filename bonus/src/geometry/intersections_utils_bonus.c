/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_inters	*new_intersection(float t, t_object *obj)
{
	t_inters	*new;

	new = malloc(sizeof(t_inters));
	if (!new)
		return (NULL);
	new->object = obj;
	new->t = t;
	new->next = NULL;
	return (new);
}

void	ft_add_intersection(t_inters **lst, t_inters *new)
{
	t_inters	*ptr;

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

void	sort_intersections(t_inters **lst)
{
	t_inters	*i1;
	t_inters	*i2;
	float		temp_t;
	t_object	*temp_obj;

	if (!lst || !*lst)
		return ;
	i1 = *lst;
	while (i1)
	{
		i2 = i1->next;
		while (i2)
		{
			if (i1->t > i2->t)
			{
				temp_t = i1->t;
				temp_obj = i1->object;
				i1->t = i2->t;
				i1->object = i2->object;
				i2->t = temp_t;
				i2->object = temp_obj;
			}
			i2 = i2->next;
		}
		i1 = i1->next;
	}
}

float	hit(t_inters *list)
{
	t_inters	*current;
	float		min_positive;

	current = list;
	min_positive = INFINITY;
	while (current)
	{
		if (current->t >= 0 && current->t < min_positive)
			min_positive = current->t;
		current = current->next;
	}
	if (min_positive == INFINITY)
		return (-1);
	return (min_positive);
}
