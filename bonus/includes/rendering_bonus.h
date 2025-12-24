/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/07 08:09:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_BONUS_H
# define RENDERING_BONUS_H

# include "types_bonus.h"
# include "../MLX42/include/MLX42/MLX42.h"

mlx_image_t	*render(mlx_t *mlx, t_camera cam, t_world world);
bool		mlx_image_to_ppm(mlx_image_t *img, char *string, \
			unsigned int range);
int			convert_color(t_tuple c);
void		execute_raytracing(void);

#endif
