/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tuple_arithmetic.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_tuple	tuple_scalar_mult(t_tuple a, float scalar)
{
	return ((t_tuple){a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar});
}

t_tuple	tuple_scalar_divide(t_tuple a, float scalar)
{
	return ((t_tuple){a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar});
}

t_tuple	mult_tuple(t_tuple a, t_tuple b)
{
	return ((t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w});
}
