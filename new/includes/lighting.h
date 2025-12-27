/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTING_H
# define LIGHTING_H

# include "types.h"

t_light		point_light(t_tuple point, t_tuple color);
t_tuple		lighting(t_material material, t_light light, t_tuple position, \
			t_tuple eyev, t_tuple normalv, bool in_shadow, \
			t_tuple ambient_color);

// Texture helpers
t_tuple		ft_get_uv_coords(t_tuple local_point, t_obj_type obj_type);
t_tuple		ft_uv_pattern_at(t_tex texture, float u, float v);
t_tuple		ft_perturb_normal_with_btex(t_material material, t_tuple normal, \
			t_tuple local_point, t_obj_type obj_type);

#endif
