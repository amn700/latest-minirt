/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tuple_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:15:53 by mohchaib          #+#    #+#             */
/*   Updated: 2025/11/24 22:28:38 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

bool	f_equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

bool	tuple_comparison(t_tuple a, t_tuple b)
{
	return (f_equal(a.x, b.x) && f_equal(a.y, b.y) && f_equal(a.z, b.z) && a.w == b.w);
}

t_tuple add_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

t_tuple substract_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

t_tuple negate_tuple(t_tuple a)
{
	return (t_tuple){-a.x, -a.y, -a.z, a.w};
}

t_tuple tuple_scalar_mult(t_tuple a, float scalar)
{
	return(t_tuple){(a.x * scalar), (a.y * scalar), (a.z * scalar), a.w};
}

t_tuple tuple_scalar_divide(t_tuple a, float scalar)
{
	if (f_equal(scalar, 0.0f))
		return(t_tuple){0, 0, 0, a.w};
	return(t_tuple){(a.x / scalar), (a.y / scalar), (a.z / scalar), a.w};
}

t_tuple mult_tuple(t_tuple a, t_tuple b)
{
    return (t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}
