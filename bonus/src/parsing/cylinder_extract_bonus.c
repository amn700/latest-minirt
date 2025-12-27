/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_extract_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	setup_cylinder_data(t_cylinder *cl, char **fields, int field_count)
{
	float	x;
	float	y;
	float	z;
	float	diameter;
	float	height;
	float	radius;
	int		color_idx;

	x = ft_atof(fields[1]);
	y = ft_atof(fields[2]);
	z = ft_atof(fields[3]);
	cl->center = (t_tuple){x, y, z, 1};
	cl->axis.x = ft_atof(fields[4]);
	cl->axis.y = ft_atof(fields[5]);
	cl->axis.z = ft_atof(fields[6]);
	cl->axis.w = 0;
	diameter = ft_atof(fields[7]);
	height = ft_atof(fields[8]);
	radius = diameter / 2.0;
	cl->diameter = diameter;
	cl->height = height;
	cl->minimum = -height / 2.0;
	cl->maximum = height / 2.0;
	cl->trans = multiply_matrix(translation(x, y, z), \
		multiply_matrix(align_y_to_vector(cl->axis), scaling(radius, height / 2.0, radius)));
	if (field_count >= 13)
	{
		cl->closed = (ft_atoi(fields[9]) == 1);
		color_idx = 10;
	}
	else
	{
		cl->closed = false;
		color_idx = 9;
	}
	cl->material.color.x = ft_atoi(fields[color_idx]) / 255.0;
	cl->material.color.y = ft_atoi(fields[color_idx + 1]) / 255.0;
	cl->material.color.z = ft_atoi(fields[color_idx + 2]) / 255.0;
	cl->material.color.w = 0;
}

bool	cylinder_extract(char *line, t_data *data)
{
	t_cylinder	cl;
	t_object	*cl_obj;
	char		**fields;
	int			field_count;
	int			color_idx;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (printf("❌ cy: Failed to split line\n"), false);
	field_count = count_fields(fields);
	if (field_count < 12)
		return (printf("❌ cy: Expected at least 12 fields, got %d\n", field_count), free_matrix(fields), false);
	if (ft_strncmp(fields[0], "cy", 3) != 0)
		return (printf("❌ cy: Invalid identifier '%s'\n", fields[0]), free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (printf("❌ cy: Invalid position (fields 1-3): %s,%s,%s\n", fields[1], fields[2], fields[3]), free_matrix(fields), false);
	if (!tuple_validator(&fields[4], false, -1.0, 1.0))
		return (printf("❌ cy: Invalid axis (fields 4-6, range -1 to 1): %s,%s,%s\n", fields[4], fields[5], fields[6]), free_matrix(fields), false);
	if (!f_field_validation(fields[7]) || \
		!f_range_validator(0.0, INFINITY, fields[7]))
		return (printf("❌ cy: Invalid diameter (field 7, must be > 0): %s\n", fields[7]), free_matrix(fields), false);
	if (!f_field_validation(fields[8]) || \
		!f_range_validator(0.0, INFINITY, fields[8]))
		return (printf("❌ cy: Invalid height (field 8, must be > 0): %s\n", fields[8]), free_matrix(fields), false);
	color_idx = (field_count >= 13) ? 10 : 9;
	if (!tuple_validator(&fields[color_idx], true, 0, 255))
		return (printf("❌ cy: Invalid color (fields %d-%d, range 0-255): %s,%s,%s\n", color_idx, color_idx+2, fields[color_idx], fields[color_idx+1], fields[color_idx+2]), free_matrix(fields), false);
	cl = cylinder();
	cl.material = material();
	setup_cylinder_data(&cl, fields, field_count);
	if (field_count > color_idx + 3 && !parse_material_params(fields, color_idx + 3, &cl.material, data->ptr))
		return (printf("❌ cy: Invalid material params\n"), free_matrix(fields), false);
	cl_obj = new_object(OBJ_CYLINDER, (t_shapes){.cy = cl});
	ft_add_object(&data->object, cl_obj);
	return (free_matrix(fields), true);
}
