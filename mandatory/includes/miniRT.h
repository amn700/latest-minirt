/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2025/12/07 13:24:58 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <float.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include "types.h"
# include "math_ops.h"
# include "geometry.h"
# include "core.h"
# include "lighting.h"
# include "parsing.h"
# include "rendering.h"
# include "utils.h"

void	free_objects_and_lights(t_data *data);

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
