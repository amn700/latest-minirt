/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_extract_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by mac               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
** Build transformation matrix for cone
** Unit cone: tip at y=0, radius at y equals |y|
** At y=1, radius=1, so for diameter d and height h:
** Scale x,z by d/2 (radius), scale y by h
** Transform order: translate * rotate * scale
*/
static t_matrix	build_cone_transform(t_tuple center, t_tuple axis,
	float diameter, float height)
{
	t_matrix	scale;
	t_matrix	align;
	t_matrix	position;
	float		radius;

	radius = diameter / 2.0f;
	scale = scaling(radius, height, radius);
	align = align_y_to_vector(axis);
	position = translation(center.x, center.y, center.z);
	return (multiply_matrix(position, multiply_matrix(align, scale)));
}

static void	setup_cone_data(t_cone *co, char **fields,
	int field_count, bool token_is_cn)
{
	int		color_idx;

	co->center = (t_tuple){
		ft_atof(fields[1]), ft_atof(fields[2]), ft_atof(fields[3]), 1};
	co->axis = normalizing_vector((t_tuple){
		ft_atof(fields[4]), ft_atof(fields[5]), ft_atof(fields[6]), 0});
	co->diameter = ft_atof(fields[7]);
	co->height = ft_atof(fields[8]);
	co->angle = atan((co->diameter / 2.0f) / co->height);
	co->minimum = 0.0f;
	co->maximum = co->height;
	co->trans = build_cone_transform(co->center, co->axis,
			co->diameter, co->height);
	if (!token_is_cn && field_count >= 13)
	{
		co->closed = (ft_atoi(fields[9]) == 1);
		color_idx = 10;
	}
	else
	{
		co->closed = true;
		color_idx = 9;
	}
	co->material.color = (t_tuple){
		ft_atoi(fields[color_idx]) / 255.0f,
		ft_atoi(fields[color_idx + 1]) / 255.0f,
		ft_atoi(fields[color_idx + 2]) / 255.0f, 0};
}

static bool	validate_cone_fields(char **fields, int field_count, bool is_cn)
{
	int	color_idx;

	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (printf("❌ co: Invalid position (fields 1-3): %s,%s,%s\n", fields[1], fields[2], fields[3]), false);
	if (!tuple_validator(&fields[4], false, -1.0, 1.0))
		return (printf("❌ co: Invalid axis (fields 4-6, range -1 to 1): %s,%s,%s\n", fields[4], fields[5], fields[6]), false);
	if (!f_field_validation(fields[7])
		|| !f_range_validator(0.0, INFINITY, fields[7]))
		return (printf("❌ co: Invalid diameter (field 7, must be > 0): %s\n", fields[7]), false);
	if (!f_field_validation(fields[8])
		|| !f_range_validator(0.0, INFINITY, fields[8]))
		return (printf("❌ co: Invalid height (field 8, must be > 0): %s\n", fields[8]), false);
	color_idx = 9;
	if (!is_cn && field_count >= 13)
		color_idx = 10;
	if (!tuple_validator(&fields[color_idx], true, 0, 255))
		return (printf("❌ co: Invalid color (fields %d-%d, range 0-255): %s,%s,%s\n", color_idx, color_idx+2, fields[color_idx], fields[color_idx+1], fields[color_idx+2]), false);
	return (true);
}

bool	cone_extract(char *line, t_data *data)
{
	t_cone		co;
	t_object	*co_obj;
	char		**fields;
	int			field_count;
	int			color_idx;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (printf("❌ co: Failed to split line\n"), false);
	field_count = count_fields(fields);
	if (field_count < 12)
		return (printf("❌ co: Expected at least 12 fields, got %d\n", field_count), free_matrix(fields), false);
	if (ft_strncmp(fields[0], "co", 3) != 0)
		return (printf("❌ co: Invalid identifier '%s'\n", fields[0]), free_matrix(fields), false);
	if (!validate_cone_fields(fields, field_count, false))
		return (free_matrix(fields), false);
	co = cone();
	setup_cone_data(&co, fields, field_count, false);
	color_idx = (field_count >= 13) ? 10 : 9;
	if (field_count > color_idx + 3
		&& !parse_material_params(fields, color_idx + 3, &co.material, data->ptr))
		return (printf("❌ co: Invalid material params\n"), free_matrix(fields), false);
	co_obj = new_object(OBJ_CONE, (t_shapes){.co = co});
	ft_add_object(&data->object, co_obj);
	return (free_matrix(fields), true);
}
