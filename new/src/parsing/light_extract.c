/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_extract.c                                    :+:      :+:    :+:   */
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

bool	extract_light(char *line, t_data *data, t_parser *parser)
{
	char	**fields;
	t_light	*light;

	if (parser->light_set)
		return (false);
	parser->light_set = true;
	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "L", 2) != 0)
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[4]) || \
		!f_range_validator(0.0, 1.0, fields[4]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (free_matrix(fields), false);
	light = new_light((t_tuple){0, 0, 0, 1}, (t_tuple){0, 0, 0, 0});
	ft_add_light(&data->light, light);
	data->light->origin.x = ft_atof(fields[1]);
	data->light->origin.y = ft_atof(fields[2]);
	data->light->origin.z = ft_atof(fields[3]);
	data->light->origin.w = 1;
	data->light->brightness = ft_atof(fields[4]);
	data->light->color.x = ft_atoi(fields[5]) / 255.0;
	data->light->color.y = ft_atoi(fields[6]) / 255.0;
	data->light->color.z = ft_atoi(fields[7]) / 255.0;
	data->light->color.w = 0;
	return (free_matrix(fields), true);
}
