/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tuple_basic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

bool	f_equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

bool	tuple_comparison(t_tuple a, t_tuple b)
{
	return (f_equal(a.x, b.x) && f_equal(a.y, b.y) \
		&& f_equal(a.z, b.z) && f_equal(a.w, b.w));
}

t_tuple	add_tuple(t_tuple a, t_tuple b)
{
	return ((t_tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w});
}

t_tuple	substract_tuple(t_tuple a, t_tuple b)
{
	return ((t_tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w});
}

t_tuple	negate_tuple(t_tuple a)
{
	return ((t_tuple){-a.x, -a.y, -a.z, -a.w});
}
