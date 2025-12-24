/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_extractors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/03 01:39:12 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

typedef struct s_parser
{
	bool	ambl_set;
	bool	cam_set;
	bool	light_set;
}	t_parser;

bool	extract_ambient_light(char *line, t_data *data, t_parser *parser);
bool	extract_camera(char *line, t_data *data, t_parser *parser);
bool	extract_light(char *line, t_data *data, t_parser *parser);
bool	sphere_extract(char *line, t_data *data);
bool	plane_extract(char *line, t_data *data);
bool	cylinder_extract(char *line, t_data *data);

static bool	extractor(char *line, t_data *data, t_parser *parser)
{
	int	i;

	i = 0;
	skip_spaces(line, &i);
	if (line[i] == 'A')
		return (extract_ambient_light(line + i, data, parser));
	else if (line[i] == 'C')
		return (extract_camera(line + i, data, parser));
	else if (line[i] == 'L')
		return (extract_light(line + i, data, parser));
	else if (ft_strncmp(line + i, "sp", 2) == 0)
		return (sphere_extract(line + i, data));
	else if (ft_strncmp(line + i, "pl", 2) == 0)
		return (plane_extract(line + i, data));
	else if (ft_strncmp(line + i, "cy", 2) == 0)
		return (cylinder_extract(line + i, data));
	return (true);
}

bool	extract_data(char *file, t_data *data)
{
	t_parser	parser;
	char		*line;
	int			fd;

	parser.ambl_set = false;
	parser.cam_set = false;
	parser.light_set = false;
	fd = open(file, O_RDONLY, 0644);
	if (fd < 0)
		return (errors_map(2), false);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (!extractor(line, data, &parser))
			return (free(line), close(fd),free_objects_and_lights(data), errors_map(3), false);
		free(line);
	}
	close(fd);
	if (!parser.ambl_set || !parser.cam_set || !parser.light_set)
		return (free_objects_and_lights(data), false);
	return (true);
}
