/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_extract_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

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
	sp->trans = multiply_matrix(translation(x, y, z), \
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
	int			field_count;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (printf("❌ sp: Failed to split line\n"), false);
	field_count = count_fields(fields);
	if (field_count < 8)
		return (printf("❌ sp: Expected 8 fields, got %d\n", field_count), free_matrix(fields), false);
	if (ft_strncmp(fields[0], "sp", 3) != 0)
		return (printf("❌ sp: Invalid identifier '%s'\n", fields[0]), free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (printf("❌ sp: Invalid position (fields 1-3): %s,%s,%s\n", fields[1], fields[2], fields[3]), free_matrix(fields), false);
	if (!f_field_validation(fields[4]) || \
		!f_range_validator(0.0, INFINITY, fields[4]))
		return (printf("❌ sp: Invalid diameter (field 4, must be > 0): %s\n", fields[4]), free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (printf("❌ sp: Invalid color (fields 5-7, range 0-255): %s,%s,%s\n", fields[5], fields[6], fields[7]), free_matrix(fields), false);
	sp = sphere();
	sp.material = material();
	setup_sphere_transform(&sp, fields);
	if (field_count > 8 && !parse_material_params(fields, 8, &sp.material, data->ptr))
		return (printf("❌ sp: Invalid material params starting at field 8\n"), free_matrix(fields), false);
	sp_obj = new_object(OBJ_SPHERE, (t_shapes){.sp = sp});
	ft_add_object(&data->object, sp_obj);
	return (free_matrix(fields), true);
}
