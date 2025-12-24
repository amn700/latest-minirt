/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

# include "types.h"

bool		f_equal(float a, float b);
bool		tuple_comparison(t_tuple a, t_tuple b);
t_tuple		add_tuple(t_tuple a, t_tuple b);
t_tuple		substract_tuple(t_tuple a, t_tuple b);
t_tuple		negate_tuple(t_tuple a);
t_tuple		tuple_scalar_mult(t_tuple a, float scalar);
t_tuple		tuple_scalar_divide(t_tuple a, float scalar);
t_tuple		mult_tuple(t_tuple a, t_tuple b);
float		vector_magnitude(t_tuple v);
t_tuple		normalizing_vector(t_tuple v);
float		vecs_dot_product(t_tuple va, t_tuple vb);
t_tuple		vecs_cross_product(t_tuple va, t_tuple vb);
t_tuple		hadamard_product(t_tuple a, t_tuple b);
t_tuple		reflect(t_tuple in, t_tuple normal);
bool		compare_matrix(t_matrix a, t_matrix b);
t_matrix	multiply_matrix(t_matrix a, t_matrix b);
t_tuple		multiply_matrix_by_tuple(t_matrix m, t_tuple v);
t_matrix	transposing_matrix(t_matrix matrix);
float		matrix_determinant_2x2(t_matrix matrix);
t_matrix	get_sub_matrix(t_matrix matrix, int row, int col);
float		get_matrix_minor_3x3(t_matrix matrix, int row, int col);
float		compute_cofactor(t_matrix matrix, int row, int col);
float		matrix_determinant(t_matrix matrix);
bool		check_if_invertible(t_matrix matrix);
t_matrix	inverse_matrix(t_matrix matrix);
t_matrix	identity(void);
t_matrix	translation(float x, float y, float z);
t_matrix	scaling(float x, float y, float z);
double		to_radians(int degrees);
t_matrix	rotation_x(float theta);
t_matrix	rotation_y(float theta);
t_matrix	rotation_z(float theta);
t_matrix	shearing(t_propo propotion);
t_matrix	align_y_to_vector(t_tuple target_axis);

#endif
