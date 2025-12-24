/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2025/12/14 23:01:57 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# include <unistd.h>
# include <float.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# ifndef MLX42_TYPES_DECL
#  define MLX42_TYPES_DECL
typedef struct mlx mlx_t;
typedef struct mlx_image mlx_image_t;
# endif
# include "get_next_line.h"
# include "libft.h"
# include "types_bonus.h"
# include "math_ops_bonus.h"
# include "geometry_bonus.h"
# include "core_bonus.h"
# include "lighting_bonus.h"
# include "parsing_bonus.h"
# include "rendering_bonus.h"
# include "utils_bonus.h"

// Patterns API
t_pattern	stripe_patern(t_tuple a, t_tuple b);
t_pattern	ring_pattern(t_tuple a, t_tuple b);
t_pattern	checkers_pattern(t_tuple a, t_tuple b);
t_tuple		stripe_at(t_pattern pattern, t_tuple point);
t_tuple		ring_at(t_pattern pattern, t_tuple point);
t_tuple		checkers_at(t_pattern pattern, t_tuple point);

void	free_objects_and_lights(t_data *data);
// # define M_PI		3.14159265358979323846

typedef struct s_data
{
	t_ambient_light	ambl;
	t_camera		cam;
	t_light			*light;
	t_object		*object;
	t_world			world;
	mlx_t			*ptr;
	mlx_image_t		*img;
}	t_data;

#endif
