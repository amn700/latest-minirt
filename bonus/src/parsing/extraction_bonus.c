/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extraction_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:53:04 by mohchaib          #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

typedef struct s_parser
{
	bool ambl_set;
	bool cam_set;
	bool light_set;	
}	t_parser;

float	ft_atof(char *str)
{
	float	result;
	float	sign;
	float	decimal_place;

	result = 0.0f;
	sign = 1.0f;
	decimal_place = 0.1f;
	if (*str == '-')
	{
		sign = -1.0f;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		result = result * 10.0f + (*str++ - '0');
	if (*str++ == '.')
	{
		while (*str >= '0' && *str <= '9')
		{
			result += (*str++ - '0') * decimal_place;
			decimal_place *= 0.1f;
		}
	}
	return (result * sign);
}

bool	i_field_validation(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

bool	f_field_validation(char *str)
{
	int i = 0;
	int dot_count = 0;

	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if ((str[i] >= '0' && str[i] <= '9'))
		{
			i++;
			continue ;
		}
		if (str[i] == '.')
		{
			dot_count++;
			if (dot_count > 1)
				return false;
		}
		i++;
	}
	return true;
}

bool	i_range_validator(int min, int max, char *line)
{
	int s = ft_atoi(line);
	if (s < min || s > max)
		return false;
	return true;
}

bool	f_range_validator(float min, float max, char *line)
{
	float f = ft_atof(line);
	if (f < min || f > max)
		return false;
	return true;
}

void	skip_spaces(char *line, int *i)
{
	while (line[*i] == 32 || (line[*i]>= 9 && line[*i]<= 13))
		(*i)++;
}


bool	check_fields_num(char **fields, int n)
{
	int i = 0;
	while (fields[i])
	{
		i++;
	}
	if (i != n)
		return (false);
	return (true);
}

bool	tuple_validator(char **fields, bool b, float min, float max)
{
	int i = 0;
	
	if (b)
	{
		while (fields[i] && i < 3)
		{
			if (!i_field_validation(fields[i]) || !i_range_validator(min, max, fields[i]))
				return (false);
			i++;
		}
	}
	else
	{
		while (fields[i] && i < 3)
		{
			if (!f_field_validation(fields[i]) || !f_range_validator(min, max, fields[i]))
				return (false);
			i++;
		}	
	}
	return (true);
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}

bool	extract_ambient_light(char *line, t_data *data, t_parser *parser)
{
	char	**fields;

	if (parser->ambl_set)
		return (false);
	parser->ambl_set = true;
	fields = ft_split_rt(line , " \t,\n");
	if (!fields)
		return (false);
	// int i;
	// printf("line: %s", line);
	// for(i = 0; fields[i]; i++)
	// 	printf("%s\n", fields[i]);
	// printf("number of fields %i\n", i);
	
	if (!check_fields_num(fields, 5))
		return (free_matrix(fields),  false);
	if (ft_strncmp(fields[0], "A", 2) != 0)
		return (free_matrix(fields),  false);
	if (!f_field_validation(fields[1]))
		return (free_matrix(fields),  false);
	if (!f_range_validator(0.0, 1.0, fields[1]))
		return (free_matrix(fields),  false);
	if (!tuple_validator(&fields[2], true, 0, 255))
		return (free_matrix(fields),  false);
	data->ambl.l_ratio = ft_atof(fields[1]);
	data->ambl.color.x = ft_atoi(fields[2]) / 255.0;
	data->ambl.color.y = ft_atoi(fields[3]) / 255.0;
	data->ambl.color.z = ft_atoi(fields[4]) / 255.0;
	data->ambl.color.w = 0;
	return (free_matrix(fields),  true);
}

bool	extract_camera(char *line, t_data *data, t_parser *parser)
{
	char	**fields;

	if (parser->cam_set)
		return (false);
	parser->cam_set = true;
	fields = ft_split_rt(line , " \t,\n");
	if (!fields)
		return (false);
	
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields),false);
	if (ft_strncmp(fields[0], "C", 2) != 0)
		return (free_matrix(fields),false);
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields),false);
	if (!tuple_validator(&fields[4], false, -1, 1))
		return (free_matrix(fields),false);
	if (!i_field_validation(fields[7]))
		return (free_matrix(fields),false);
	if (!i_range_validator(0, 180, fields[7]))
			return (free_matrix(fields),false);
	// Parse camera position
	data->cam.from.x = ft_atof(fields[1]);
	data->cam.from.y = ft_atof(fields[2]);
	data->cam.from.z = ft_atof(fields[3]);
	data->cam.from.w = 1;
	
	// Parse orientation vector (direction camera is pointing)
	t_tuple direction;
	direction.x = ft_atof(fields[4]);
	direction.y = ft_atof(fields[5]);
	direction.z = ft_atof(fields[6]);
	direction.w = 0;
	
	// Calculate 'to' point: position + normalized direction
	t_tuple normalized_dir = normalizing_vector(direction);
	data->cam.to = add_tuple(data->cam.from, normalized_dir);
	
	// Set up vector (Y-axis by default)
	data->cam.up = (t_tuple){0, 1, 0, 0};
	
	// Store FOV
	data->cam.fov = ft_atoi(fields[7]);
	
	return (free_matrix(fields), true);
}

bool	extract_light(char *line, t_data *data, t_parser *parser)
{
	char	**fields;

	if (parser->light_set)
		return (false);
	parser->light_set = true;
	fields = ft_split_rt(line , " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "L", 2) != 0)
		return (free_matrix(fields), false);

	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[4]) || !f_range_validator(0.0, 1.0, fields[4]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (free_matrix(fields), false);
	t_light *light = new_light((t_tuple){0,0,0,1}, (t_tuple){0,0,0,0});
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

bool	sphere_extract(char *line, t_data *data)
{
	char	**fields;
	t_sphere sp;
	t_object *sp_obj;

	fields = ft_split_rt(line , " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 8))
		return (free_matrix(fields), false);
	if (ft_strncmp(fields[0], "sp", 3) != 0)
		return (free_matrix(fields), false);	
	if (!tuple_validator(&fields[1], false, -INFINITY, INFINITY))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[4]) || !f_range_validator(0.0, INFINITY, fields[4]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[5], true, 0, 255))
		return (free_matrix(fields), false);
	sp = sphere();
	sp.material = material();
	
	// Parse position and diameter
	float x = ft_atof(fields[1]);
	float y = ft_atof(fields[2]);
	float z = ft_atof(fields[3]);
	float diameter = ft_atof(fields[4]);
	float radius = diameter / 2.0;
	
	// Store diameter for reference
	sp.diam = diameter;
	// Sphere center is always at origin in object space (transformation handles position)
	sp.center = (t_tuple){0, 0, 0, 1};
	
	// Apply transformation: scale by radius, then translate to position
	sp.trans = multiply_matrix(translation(x, y, z), scaling(radius, radius, radius));
	sp.trans_inv = inverse_matrix(sp.trans);
	
	// Parse color
	sp.material.color.x = ft_atoi(fields[5]) / 255.0;
	sp.material.color.y = ft_atoi(fields[6]) / 255.0;
	sp.material.color.z = ft_atoi(fields[7]) / 255.0;
	sp.material.color.w = 0;
	
	sp_obj = new_object(OBJ_SPHERE, (t_shapes){.sp = sp});
	ft_add_object(&data->object, sp_obj);
	return (free_matrix(fields), true);
}

bool	plane_extract(char *line, t_data *data)
{
	t_plane		pl;
	t_object	*pl_obj;
	char		**fields;

	fields = ft_split_rt(line , " \t,\n");
	if (!fields)
		return (false);
	if (!check_fields_num(fields, 10))
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
	
	// Parse position
	float x = ft_atof(fields[1]);
	float y = ft_atof(fields[2]);
	float z = ft_atof(fields[3]);
	
	// Store point position
	pl.point = (t_tuple){x, y, z, 1};
	
	// Parse and store normal (for transformation calculation)
	pl.normal.x = ft_atof(fields[4]);
	pl.normal.y = ft_atof(fields[5]);
	pl.normal.z = ft_atof(fields[6]);
	pl.normal.w = 0;
	pl.normal = normalizing_vector(pl.normal);
	
	// Default plane is at y=0 with normal (0,1,0)
	// We need to rotate to match the given normal, then translate
	t_matrix rotation = align_y_to_vector(pl.normal);
	pl.trans = multiply_matrix(translation(x, y, z), rotation);
	pl.trans_inv = inverse_matrix(pl.trans);
	
	// Parse color
	pl.material.color.x = ft_atoi(fields[7]) / 255.0;
	pl.material.color.y = ft_atoi(fields[8]) / 255.0;
	pl.material.color.z = ft_atoi(fields[9]) / 255.0;
	pl.material.color.w = 0;
	
	pl_obj = new_object(OBJ_PLANE, (t_shapes){.pl = pl});
	ft_add_object(&data->object, pl_obj);
	return (free_matrix(fields), true);
}

bool	cylinder_extract(char *line, t_data *data)
{
	t_cylinder		cl;
	t_object	*cl_obj;
	char		**fields;

	fields = ft_split_rt(line , " \t,\n");
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
	if (!f_field_validation(fields[7]) || !f_range_validator(0.0, INFINITY, fields[7]))
		return (free_matrix(fields), false);
	if (!f_field_validation(fields[8]) || !f_range_validator(0.0, INFINITY, fields[8]))
		return (free_matrix(fields), false);
	if (!tuple_validator(&fields[9], true, 0, 255))
		return (free_matrix(fields), false);
	cl = cylinder();
	cl.material = material();
	
	// Parse position
	float x = ft_atof(fields[1]);
	float y = ft_atof(fields[2]);
	float z = ft_atof(fields[3]);
	
	// Store center
	cl.center = (t_tuple){x, y, z, 1};
	
	// Parse axis and dimensions
	cl.axis.x = ft_atof(fields[4]);
	cl.axis.y = ft_atof(fields[5]);
	cl.axis.z = ft_atof(fields[6]);
	cl.axis.w = 0;
	
	float diameter = ft_atof(fields[7]);
	float height = ft_atof(fields[8]);
	float radius = diameter / 2.0;
	
	// Store diameter and height
	cl.diameter = diameter;
	cl.height = height;
	
	// Default cylinder is along Y-axis, radius 1, height from -inf to +inf
	// Set minimum and maximum based on height
	cl.minimum = -height / 2.0;
	cl.maximum = height / 2.0;
	
	// Apply transformation: scale radius, then translate to position
	cl.trans = multiply_matrix(translation(x, y, z), scaling(radius, 1, radius));
	cl.trans_inv = inverse_matrix(cl.trans);
	
	// Parse color
	cl.material.color.x = ft_atoi(fields[9]) / 255.0;
	cl.material.color.y = ft_atoi(fields[10]) / 255.0;
	cl.material.color.z = ft_atoi(fields[11]) / 255.0;
	cl.material.color.w = 0;
	
	cl_obj = new_object(OBJ_CYLINDER, (t_shapes){.cy = cl});
	ft_add_object(&data->object, cl_obj);
	return (free_matrix(fields), true);	
}

bool	extractor(char *line, t_data *data, t_parser *parser)
{
	int	i;

	i = 0;
	skip_spaces(line, &i);
	if (line[i] == 'A')
	{
		if (!extract_ambient_light(line + i, data, parser))
			return (false);	
	}
	else if (line[i] == 'C')
	{
		if (!extract_camera(line + i, data, parser))
			return (false);
	}
	else if (line[i] == 'L')
	{
		if (!extract_light(line + i, data, parser))
			return (false);
	}
	else if (ft_strncmp(line + i, "sp", 2) == 0)
	{
		if (!sphere_extract(line + i, data))
			return (false);
	}
	else if (ft_strncmp(line + i, "pl", 2) == 0)
	{
		if (!plane_extract(line + i, data))
			return (false);
	}
	else if (ft_strncmp(line + i, "cy", 2) == 0)
	{
		if (!cylinder_extract(line + i, data))
			return (false);
	}
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
	fd = open (file, O_RDONLY, 0644);
	if (fd < 0)
		return (errors_map(2), false);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (!extractor(line, data, &parser))
			return (free(line), close(fd), errors_map(3), false);
		free(line);
	}
	close(fd);
	if (!parser.ambl_set || !parser.cam_set || !parser.light_set)
		return (false);
	return (true);
}
