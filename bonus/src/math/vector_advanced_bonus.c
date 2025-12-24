/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_advanced_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/07 13:02:43 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

t_tuple	vecs_cross_product(t_tuple va, t_tuple vb)
{
	return ((t_tuple){va.y * vb.z - va.z * vb.y, va.z * vb.x - va.x * vb.z, \
		va.x * vb.y - va.y * vb.x, 0});
}

t_tuple	hadamard_product(t_tuple a, t_tuple b)
{
	return ((t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, 0});
}

t_tuple	reflect(t_tuple in, t_tuple normal)
{
	return (substract_tuple(in, tuple_scalar_mult(normal, \
		2 * vecs_dot_product(in, normal))));
}
