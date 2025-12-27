/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	setup_plane_data(t_plane *pl, char **fields)
{
	float		x;
	float		y;
	float		z;
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

static void	parse_plane_textures(t_plane *pl, char **fields)
{
	int	i;

	i = 10;
	while (fields[i])
	{
		parse_texture_param(fields[i], &pl->material);
		i++;
	}
}

bool	plane_extract(char *line, t_data *data)
{
	t_plane		pl;
	t_object	*pl_obj;
	char		**fields;
	int			field_count;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	field_count = 0;
	while (fields[field_count])
		field_count++;
	if (field_count < 10)
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "pl", 3) != 0)
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[4], false, -1.0, 1.0))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[7], true, 0, 255))
		return (free_matrix(fields), false);
	pl = plane();
	pl.material = material();
	setup_plane_data(&pl, fields);
	parse_plane_textures(&pl, fields);
	pl_obj = new_object(OBJ_PLANE, (t_shapes){.pl = pl});
	ft_add_object(&data->object, pl_obj);
	return (free_matrix(fields), true);
}
