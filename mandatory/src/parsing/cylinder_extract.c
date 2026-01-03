/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_extract.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 09:01:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	setup_cylinder_data(t_cylinder *cl, char **fields)
{
	float	x;
	float	y;
	float	z;
	float	diameter;
	float	height;
	float	radius;

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
		scaling(radius, 1, radius));
	cl->material.color.x = ft_atoi(fields[9]) / 255.0;
	cl->material.color.y = ft_atoi(fields[10]) / 255.0;
	cl->material.color.z = ft_atoi(fields[11]) / 255.0;
	cl->material.color.w = 0;
}

bool	cylinder_extract(char *line, t_data *data)
{
	t_cylinder	cl;
	t_object	*cl_obj;
	char		**fields;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 12))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "cy", 3) != 0)
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[4], false, -1.0, 1.0))
		return (free_matrix(fields), false);
	if (ft_atof(fields[4]) == 0 && ft_atof(fields[5]) == 0 \
		&& ft_atof(fields[6]) == 0)
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[7]) || \
		!f_range_validator(0.0, INFINITY, fields[7]))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[8]) || \
		!f_range_validator(0.0, INFINITY, fields[8]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[9], true, 0, 255))
		return (free_matrix(fields), false);
	cl = cylinder();
	cl.material = material();
	setup_cylinder_data(&cl, fields);
	cl_obj = new_object(OBJ_CYLINDER, (t_shapes){.cy = cl});
	ft_add_object(&data->object, cl_obj);
	return (free_matrix(fields), true);
}
