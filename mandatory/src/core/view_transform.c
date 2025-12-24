/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_transform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_matrix	orientation(t_tuple left, t_tuple true_up, t_tuple forward)
{
	t_matrix	ort;

	ort.mtrx[0][0] = left.x;
	ort.mtrx[0][1] = left.y;
	ort.mtrx[0][2] = left.z;
	ort.mtrx[0][3] = 0;
	ort.mtrx[1][0] = true_up.x;
	ort.mtrx[1][1] = true_up.y;
	ort.mtrx[1][2] = true_up.z;
	ort.mtrx[1][3] = 0;
	ort.mtrx[2][0] = -forward.x;
	ort.mtrx[2][1] = -forward.y;
	ort.mtrx[2][2] = -forward.z;
	ort.mtrx[2][3] = 0;
	ort.mtrx[3][0] = 0;
	ort.mtrx[3][1] = 0;
	ort.mtrx[3][2] = 0;
	ort.mtrx[3][3] = 1;
	return (ort);
}

t_matrix	view_transformation(t_tuple from, t_tuple to, t_tuple up)
{
	t_tuple		forward;
	t_tuple		left;
	t_tuple		true_up;
	t_matrix	orient;

	forward = normalizing_vector(substract_tuple(to, from));
	left = vecs_cross_product(forward, normalizing_vector(up));
	true_up = vecs_cross_product(left, forward);
	orient = orientation(left, true_up, forward);
	orient.size = 4;
	return (multiply_matrix(orient, translation(-from.x, -from.y, -from.z)));
}
