/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   axis_align.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by mac               #+#    #+#             */
/*   Updated: 2026/01/03 10:48:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Get quaternion from rotation axis and angle
*/
static t_tuple	get_quaternion(t_tuple rot_axis, float radian)
{
	float	sine;

	radian /= 2.0f;
	sine = sinf(radian);
	return ((t_tuple){
		cosf(radian),
		rot_axis.x * sine,
		rot_axis.y * sine,
		rot_axis.z * sine
	});
}

/*
** Convert quaternion to rotation matrix
*/
static t_matrix	quat_to_matrix(t_tuple q)
{
	t_matrix	m;

	m.size = 4;
	m.mtrx[0][0] = 1 - 2 * (q.z * q.z + q.w * q.w);
	m.mtrx[0][1] = 2 * (q.y * q.z - q.x * q.w);
	m.mtrx[0][2] = 2 * (q.y * q.w + q.x * q.z);
	m.mtrx[0][3] = 0;
	m.mtrx[1][0] = 2 * (q.y * q.z + q.x * q.w);
	m.mtrx[1][1] = 1 - 2 * (q.y * q.y + q.w * q.w);
	m.mtrx[1][2] = 2 * (q.z * q.w - q.x * q.y);
	m.mtrx[1][3] = 0;
	m.mtrx[2][0] = 2 * (q.y * q.w - q.x * q.z);
	m.mtrx[2][1] = 2 * (q.z * q.w + q.x * q.y);
	m.mtrx[2][2] = 1 - 2 * (q.y * q.y + q.z * q.z);
	m.mtrx[2][3] = 0;
	m.mtrx[3][0] = 0;
	m.mtrx[3][1] = 0;
	m.mtrx[3][2] = 0;
	m.mtrx[3][3] = 1;
	return (m);
}

t_matrix	align_y_to_vector(t_tuple target_axis)
{
	float		dot;
	float		angle;
	t_tuple		rot_axis;
	t_tuple		q;

	dot = vecs_dot_product((t_tuple){0, 1, 0, 0},
			normalizing_vector(target_axis));
	if (fabs(dot - 1.0f) < EPSILON)
		return (identity());
	if (fabs(dot + 1.0f) < EPSILON)
		return (scaling(1.0f, -1.0f, 1.0f));
	rot_axis = normalizing_vector(vecs_cross_product((t_tuple){0, 1, 0, 0},
				normalizing_vector(target_axis)));
	angle = acosf(dot);
	q = get_quaternion(rot_axis, angle);
	return (quat_to_matrix(q));
}
