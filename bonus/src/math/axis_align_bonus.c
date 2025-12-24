/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   axis_align_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by mac               #+#    #+#             */
/*   Updated: 2025/12/21 06:39:04 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
** Get quaternion from rotation axis and angle
** Reference: ft_get_quat from m4nnb3ll/miniRT
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
** Reference: ft_quat_to_matrix from m4nnb3ll/miniRT
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

/*
** Get rotation matrix to align v1 to v2 using quaternions
** Reference: ft_get_rotation_matrix from m4nnb3ll/miniRT
** v1 and v2 should both be normalized
** When vectors are parallel/anti-parallel, return identity or flip
*/
t_matrix	align_y_to_vector(t_tuple target_axis)
{
	t_tuple		v1;
	t_tuple		v2;
	float		dot;
	float		angle;
	t_tuple		rot_axis;
	t_tuple		q;

	v1 = (t_tuple){0, 1, 0, 0};
	v2 = normalizing_vector(target_axis);
	dot = vecs_dot_product(v1, v2);
	if (fabs(dot - 1.0f) < EPSILON)
		return (identity());
	if (fabs(dot + 1.0f) < EPSILON)
		return (scaling(1.0f, -1.0f, 1.0f));
	rot_axis = normalizing_vector(vecs_cross_product(v1, v2));
	angle = acosf(dot);
	q = get_quaternion(rot_axis, angle);
	return (quat_to_matrix(q));
}
