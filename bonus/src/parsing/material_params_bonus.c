/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_params_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/14 23:02:03 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static bool	parse_reflection(char *param, float *reflective)
{
	char	*value;

	if (ft_strncmp(param, "ref:", 4) != 0)
		return (false);
	value = param + 4;
	if (!f_field_validation(value))
		return (false);
	*reflective = ft_atof(value);
	if (*reflective < 0.0 || *reflective > 1.0)
		return (false);
	return (true);
}

static bool	parse_pattern_type(char *param, t_material *mat)
{
	if (ft_strncmp(param, "pat:", 4) != 0)
		return (false);
	if (ft_strncmp(param + 4, "stripe", 7) == 0)
		mat->pattern = stripe_patern(mat->pattern.a, mat->pattern.b);
	else if (ft_strncmp(param + 4, "ring", 5) == 0)
		mat->pattern = ring_pattern(mat->pattern.a, mat->pattern.b);
	else if (ft_strncmp(param + 4, "checkers", 9) == 0)
		mat->pattern = checkers_pattern(mat->pattern.a, mat->pattern.b);
	else
		return (false);
	return (true);
}

static bool	parse_pattern_color(char *param, t_tuple *dst)
{
	char **parts;

	if (!(ft_strncmp(param, "pa:", 3) == 0 || ft_strncmp(param, "pb:", 3) == 0))
		return (false);
	parts = ft_split_rt(param + 3, ":");
	if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
		return (free_matrix(parts), false);
	if (!i_field_validation(parts[0]) || !i_field_validation(parts[1]) || !i_field_validation(parts[2]))
		return (free_matrix(parts), false);
	if (!i_range_validator(0, 255, parts[0]) || !i_range_validator(0, 255, parts[1]) || !i_range_validator(0, 255, parts[2]))
		return (free_matrix(parts), false);
	dst->x = ft_atoi(parts[0]) / 255.0f;
	dst->y = ft_atoi(parts[1]) / 255.0f;
	dst->z = ft_atoi(parts[2]) / 255.0f;
	dst->w = 0;
	return (free_matrix(parts), true);
}

static bool	parse_pattern_transform(char *param, t_pattern *pat)
{
	char **parts;
	t_matrix m;

	if (ft_strncmp(param, "ptrans:", 7) == 0)
	{
		parts = ft_split_rt(param + 7, ":");
		if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
			return (free_matrix(parts), false);
		if (!f_field_validation(parts[0]) || !f_field_validation(parts[1]) || !f_field_validation(parts[2]))
			return (free_matrix(parts), false);
		m = translation(ft_atof(parts[0]), ft_atof(parts[1]), ft_atof(parts[2]));
		pat->transform = multiply_matrix(pat->transform, m);
		pat->has_transform = true;
		if (check_if_invertible(pat->transform))
			pat->inv_transform = inverse_matrix(pat->transform);
		else
			pat->has_transform = false;
		return (free_matrix(parts), true);
	}
	if (ft_strncmp(param, "pscale:", 7) == 0)
	{
		parts = ft_split_rt(param + 7, ":");
		if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
			return (free_matrix(parts), false);
		if (!f_field_validation(parts[0]) || !f_field_validation(parts[1]) || !f_field_validation(parts[2]))
			return (free_matrix(parts), false);
		m = scaling(ft_atof(parts[0]), ft_atof(parts[1]), ft_atof(parts[2]));
		pat->transform = multiply_matrix(pat->transform, m);
		pat->has_transform = true;
		if (check_if_invertible(pat->transform))
			pat->inv_transform = inverse_matrix(pat->transform);
		else
			pat->has_transform = false;
		return (free_matrix(parts), true);
	}
	if (ft_strncmp(param, "prot_x:", 7) == 0)
	{
		if (!f_field_validation(param + 7))
			return (false);
		m = rotation_x(to_radians((int)ft_atof(param + 7)));
		pat->transform = multiply_matrix(pat->transform, m);
		pat->has_transform = true;
		if (check_if_invertible(pat->transform))
			pat->inv_transform = inverse_matrix(pat->transform);
		else
			pat->has_transform = false;
		return (true);
	}
	if (ft_strncmp(param, "prot_y:", 7) == 0)
	{
		if (!f_field_validation(param + 7))
			return (false);
		m = rotation_y(to_radians((int)ft_atof(param + 7)));
		pat->transform = multiply_matrix(pat->transform, m);
		pat->has_transform = true;
		if (check_if_invertible(pat->transform))
			pat->inv_transform = inverse_matrix(pat->transform);
		else
			pat->has_transform = false;
		return (true);
	}
	if (ft_strncmp(param, "prot_z:", 7) == 0)
	{
		if (!f_field_validation(param + 7))
			return (false);
		m = rotation_z(to_radians((int)ft_atof(param + 7)));
		pat->transform = multiply_matrix(pat->transform, m);
		pat->has_transform = true;
		if (check_if_invertible(pat->transform))
			pat->inv_transform = inverse_matrix(pat->transform);
		else
			pat->has_transform = false;
		return (true);
	}
	return (false);
}

static bool	parse_refraction(char *param, float *transparency, float *refract_index)
{
	char	**parts;
	bool	valid;

	if (ft_strncmp(param, "refr:", 5) != 0)
		return (false);
	parts = ft_split_rt(param + 5, ":");
	if (!parts || !parts[0] || !parts[1] || parts[2])
		return (free_matrix(parts), false);
	if (!f_field_validation(parts[0]) || !f_field_validation(parts[1]))
		return (free_matrix(parts), false);
	*transparency = ft_atof(parts[0]);
	*refract_index = ft_atof(parts[1]);
	valid = (*transparency >= 0.0 && *transparency <= 1.0 && 
			*refract_index >= 1.0 && *refract_index <= 3.0);
	return (free_matrix(parts), valid);
}

static bool	parse_shininess(char *param, float *specular, float *shininess)
{
	char	**parts;
	bool	valid;

	if (ft_strncmp(param, "shine:", 6) != 0)
		return (false);
	parts = ft_split_rt(param + 6, ":");
	if (!parts || !parts[0])
		return (free_matrix(parts), false);
	if (!f_field_validation(parts[0]))
		return (free_matrix(parts), false);
	*specular = -1.0;
	if (parts[1])
	{
		if (!f_field_validation(parts[1]) || parts[2])
			return (free_matrix(parts), false);
		*specular = ft_atof(parts[0]);
		*shininess = ft_atof(parts[1]);
		valid = (*specular >= 0.0 && *specular <= 1.0 && 
				*shininess >= 1.0 && *shininess <= 1000.0);
	}
	else
	{
		*shininess = ft_atof(parts[0]);
		valid = (*shininess >= 1.0 && *shininess <= 1000.0);
	}
	return (free_matrix(parts), valid);
}

bool	parse_material_params(char **fields, int start_idx, t_material *mat)
{
	int		i;
	bool    saw_pattern;
	float	reflective;
	float	transparency;
	float	refract_index;
	float	specular;
	float	shininess;

	i = start_idx;
	mat->pattern.a = mat->color;
	mat->pattern.b = mat->color;
	mat->pattern.transform = identity();
	mat->pattern.has_transform = false;
	mat->pattern.at = NULL;
	saw_pattern = false;
	while (fields[i])
	{
		if (parse_pattern_type(fields[i], mat))
			saw_pattern = true;
		else if (ft_strncmp(fields[i], "pa:", 3) == 0 && parse_pattern_color(fields[i], &mat->pattern.a))
			;
		else if (ft_strncmp(fields[i], "pb:", 3) == 0 && parse_pattern_color(fields[i], &mat->pattern.b))
			;
		else if (parse_pattern_transform(fields[i], &mat->pattern))
			;
		else if (parse_reflection(fields[i], &reflective))
			mat->reflective = reflective;
		else if (parse_refraction(fields[i], &transparency, &refract_index))
		{
			mat->transparency = transparency;
			mat->refract_index = refract_index;
		}
		else if (parse_shininess(fields[i], &specular, &shininess))
		{
			if (specular >= 0.0)
				mat->specular = specular;
			mat->shininess = shininess;
		}
		else
			return (false);
		i++;
	}
	if (saw_pattern && mat->pattern.at == NULL)
		mat->pattern = stripe_patern(mat->pattern.a, mat->pattern.b);
	return (true);
}

int	count_fields(char **fields)
{
	int	count;

	count = 0;
	while (fields[count])
		count++;
	return (count);
}
