/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_extract_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	setup_plane_data(t_plane *pl, char **fields)
{
	float	x;
	float	y;
	float	z;
	t_matrix	rotation;

	x = ft_atof(fields[1]);
	y = ft_atof(fields[2]);
	z = ft_atof(fields[3]);
	pl->point = (t_tuple){x, y, z, 1};
	pl->normal.x = ft_atof(fields[4]);
	pl->normal.y = ft_atof(fields[5]);
	pl->normal.z = ft_atof(fields[6]);
	pl->normal.w = 0;
	pl->normal = normalizing_vector(pl->normal);
	rotation = align_y_to_vector(pl->normal);
	pl->trans = multiply_matrix(translation(x, y, z), rotation);
	pl->material.color.x = ft_atoi(fields[7]) / 255.0;
	pl->material.color.y = ft_atoi(fields[8]) / 255.0;
	pl->material.color.z = ft_atoi(fields[9]) / 255.0;
	pl->material.color.w = 0;
}

bool	plane_extract(char *line, t_data *data)
{
	t_plane		pl;
	t_object	*pl_obj;
	char		**fields;
	int			field_count;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (printf("❌ pl: Failed to split line\n"), false);
	field_count = count_fields(fields);
	if (field_count < 10)
		return (printf("❌ pl: Expected 10 fields, got %d\n", field_count), free_matrix(fields), false);
	if (ft_strncmp(fields[0], "pl", 3) != 0)
		return (printf("❌ pl: Invalid identifier '%s'\n", fields[0]), free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (printf("❌ pl: Invalid position (fields 1-3): %s,%s,%s\n", fields[1], fields[2], fields[3]), free_matrix(fields), false);
	if (!tuple_validator(&fields[4], false, -1.0, 1.0))
		return (printf("❌ pl: Invalid normal (fields 4-6, range -1 to 1): %s,%s,%s\n", fields[4], fields[5], fields[6]), free_matrix(fields), false);
	if (!tuple_validator(&fields[7], true, 0, 255))
		return (printf("❌ pl: Invalid color (fields 7-9, range 0-255): %s,%s,%s\n", fields[7], fields[8], fields[9]), free_matrix(fields), false);
	pl = plane();
	pl.material = material();
	setup_plane_data(&pl, fields);
	if (field_count > 10 && !parse_material_params(fields, 10, &pl.material, data->ptr))
		return (printf("❌ pl: Invalid material params starting at field 10\n"), free_matrix(fields), false);
	pl_obj = new_object(OBJ_PLANE, (t_shapes){.pl = pl});
	ft_add_object(&data->object, pl_obj);
	return (free_matrix(fields), true);
}
