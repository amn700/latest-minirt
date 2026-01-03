/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_extract_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

typedef struct s_parser
{
	bool	ambl_set;
	bool	cam_set;
	bool	light_set;
	bool	single_light_set;
}	t_parser;

bool	extract_light(char *line, t_data *data, t_parser *parser)
{
	char	**fields;
	t_light	*light;
	t_tuple	position;
	t_tuple	color;

	fields = ft_split_rt(line, " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "L", 2) == 0)
	{
		if (parser->single_light_set)
			return (printf("❌ Error: Only one 'L' light allowed\n"), 
				free_matrix(fields), false);
		parser->single_light_set = true;
	}
	else if (ft_strncmp(fields[0], "l", 2) != 0)
		return (free_matrix(fields), false);
	parser->light_set = true;
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[4]) || \
		!f_range_validator(0.0, 1.0, fields[4]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (free_matrix(fields), false);
	position = (t_tuple){ft_atof(fields[1]), ft_atof(fields[2]), \
		ft_atof(fields[3]), 1};
	color = (t_tuple){ft_atoi(fields[5]) / 255.0, ft_atoi(fields[6]) / 255.0, \
		ft_atoi(fields[7]) / 255.0, 0};
	light = new_light(position, color);
	light->brightness = ft_atof(fields[4]);
	ft_add_light(&data->light, light);
	return (free_matrix(fields), true);
}
