/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2026/01/03 07:26:01 by amn              ###   ########.fr       */
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
# include <pthread.h>
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
# include "texture_bonus.h"

# define NUM_THREADS 8
# define MODE_PREVIEW 1
# define MODE_LOAD 2

t_pattern	stripe_patern(t_tuple a, t_tuple b);
t_pattern	ring_pattern(t_tuple a, t_tuple b);
t_pattern	checkers_pattern(t_tuple a, t_tuple b);
t_tuple		stripe_at(t_pattern pattern, t_tuple point);
t_tuple		ring_at(t_pattern pattern, t_tuple point);
t_tuple		checkers_at(t_pattern pattern, t_tuple point);

void		free_objects_and_lights(t_data *data);

typedef enum e_state
{
	STATE_MENU,
	STATE_RENDERING,
	STATE_WAITING,
	STATE_DISPLAY
}	t_state;

typedef struct s_render_ctx
{
	mlx_image_t		*img;
	t_camera		cam;
	t_world			world;
	int				start_y;
	int				end_y;
	int				*lines_done;
	pthread_mutex_t	*mutex;
	volatile int	*stop_flag;
}	t_render_ctx;

typedef struct s_data
{
	t_ambient_light	ambl;
	t_camera		cam;
	t_light			*light;
	t_object		*object;
	t_world			world;
	mlx_t			*ptr;
	mlx_image_t		*img;
	mlx_image_t		*front;
	mlx_image_t		*bar_bg;
	mlx_image_t		*bar_fill;
	mlx_image_t		*bar_text;
	int				render_mode;
	t_state			state;
	int				lines_done;
	int				total_lines;
	pthread_mutex_t	render_mutex;
	pthread_t		threads[NUM_THREADS];
	t_render_ctx	thread_ctx[NUM_THREADS];
	volatile int	stop_flag;
	bool			render_complete;
	bool			threads_started;
	bool			mutex_initialized;
}	t_data;

#endif
