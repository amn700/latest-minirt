/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_extract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:17:44 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	setup_sphere_transform(t_sphere *sp, char **fields)
{
	float	x;
	float	y;
	float	z;
	float	diameter;
	float	radius;

	x = ft_atof(fields[1]);
	y = ft_atof(fields[2]);
	z = ft_atof(fields[3]);
	diameter = ft_atof(fields[4]);
	radius = diameter / 2.0;
	sp->diam = diameter;
	sp->center = (t_tuple){0, 0, 0, 1};
	sp->trans = multiply_matrix(translation(x, y, z),
			scaling(radius, radius, radius));
	sp->material.color.x = ft_atoi(fields[5]) / 255.0;
	sp->material.color.y = ft_atoi(fields[6]) / 255.0;
	sp->material.color.z = ft_atoi(fields[7]) / 255.0;
	sp->material.color.w = 0;
}

bool	sphere_extract(char *line, t_data *data)
{
	char		**fields;
	t_sphere	sp;
	t_object	*sp_obj;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "sp", 3) != 0)
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[4])
		|| !f_range_validator(0.0, INFINITY, fields[4]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (free_matrix(fields), false);
	sp = sphere();
	sp.material = material();
	setup_sphere_transform(&sp, fields);
	sp_obj = new_object(OBJ_SPHERE, (t_shapes){.sp = sp});
	ft_add_object(&data->object, sp_obj);
	return (free_matrix(fields), true);
}
