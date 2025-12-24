/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/07 08:09:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTING_BONUS_H
# define LIGHTING_BONUS_H

# include "types_bonus.h"

t_light		point_light(t_tuple point, t_tuple color);
t_tuple		lighting(t_material material, t_light light, t_tuple position, \
			t_tuple eyev, t_tuple normalv, bool in_shadow, \
			t_tuple ambient_color);

#endif
