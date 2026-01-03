/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_BONUS_H
# define RENDERING_BONUS_H

# include "types_bonus.h"
# include "../MLX42/include/MLX42/MLX42.h"

typedef struct s_data	t_data;

mlx_image_t	*render(mlx_t *mlx, t_camera cam, t_world world);
bool		mlx_image_to_ppm(mlx_image_t *img, char *string, \
			unsigned int range);
int			convert_color(t_tuple c);
void		execute_raytracing(void);

void		start_threaded_render(t_data *data);
void		stop_threaded_render(t_data *data);
void		*render_thread_func(void *arg);
uint32_t	tuple_to_pixel(t_tuple color);

void		draw_progress_bar(t_data *data);
void		draw_front_page(t_data *data);
void		update_progress_bar(t_data *data);
void		hide_ui_elements(t_data *data);
void		show_rendered_image(t_data *data);

#endif
