/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2026/01/03 12:03:05 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include "core.h"
# include "geometry.h"
# include "lighting.h"
# include "math_ops.h"
# include "parsing.h"
# include "rendering.h"
# include "types.h"
# include "utils.h"
# include <fcntl.h>
# include <float.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_data
{
	t_ambient_light	ambl;
	t_camera		cam;
	t_light			*light;
	t_object		*object;
	t_world			world;
	mlx_t			*ptr;
	mlx_image_t		*img;
}					t_data;

int					count_words(char *str, char *charset);
int					populate(char **matrix, char *str, char *charset);
void				free_objects_and_lights(t_data *data);

bool				extract_ambient_light(char *line, t_data *data,
						t_parser *parser);
bool				extract_camera(char *line, t_data *data, t_parser *parser);
bool				extract_light(char *line, t_data *data, t_parser *parser);
bool				sphere_extract(char *line, t_data *data);
bool				plane_extract(char *line, t_data *data);
bool				cylinder_extract(char *line, t_data *data);
#endif
