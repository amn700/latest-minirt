/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient_camera_extract.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

typedef struct s_parser
{
	bool	ambl_set;
	bool	cam_set;
	bool	light_set;
}	t_parser;

bool	extract_ambient_light(char *line, t_data *data, t_parser *parser)
{
	char	**fields;

	if (parser->ambl_set)
		return (false);
	parser->ambl_set = true;
	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 5))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "A", 2) != 0)
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[1]))
		return (free_matrix(fields), false);
	if (!f_range_validator(0.0, 1.0, fields[1]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[2], true, 0, 255))
		return (free_matrix(fields), false);
	data->ambl.l_ratio = ft_atof(fields[1]);
	data->ambl.color.x = ft_atoi(fields[2]) / 255.0;
	data->ambl.color.y = ft_atoi(fields[3]) / 255.0;
	data->ambl.color.z = ft_atoi(fields[4]) / 255.0;
	data->ambl.color.w = 0;
	return (free_matrix(fields), true);
}

static void	setup_camera_data(t_data *data, char **fields)
{
	t_tuple	direction;
	t_tuple	normalized_dir;

	data->cam.from.x = ft_atof(fields[1]);
	data->cam.from.y = ft_atof(fields[2]);
	data->cam.from.z = ft_atof(fields[3]);
	data->cam.from.w = 1;
	direction.x = ft_atof(fields[4]);
	direction.y = ft_atof(fields[5]);
	direction.z = ft_atof(fields[6]);
	direction.w = 0;
	normalized_dir = normalizing_vector(direction);
	data->cam.to = add_tuple(data->cam.from, normalized_dir);
	data->cam.up = (t_tuple){0, 1, 0, 0};
	data->cam.fov = ft_atoi(fields[7]);
}

bool	extract_camera(char *line, t_data *data, t_parser *parser)
{
	char	**fields;

	if (parser->cam_set)
		return (false);
	parser->cam_set = true;
	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "C", 2) != 0)
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[4], false, -1, 1))
		return (free_matrix(fields), false);
	if (!i_field_validation(fields[7]))
		return (free_matrix(fields), false);
	if (!i_range_validator(0, 180, fields[7]))
		return (free_matrix(fields), false);
	setup_camera_data(data, fields);
	return (free_matrix(fields), true);
}
