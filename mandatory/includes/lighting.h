/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:48:08 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTING_H
# define LIGHTING_H

# include "types.h"

typedef struct s_light_vars
{
	t_tuple		ambient;
	t_tuple		effective_color;
	t_tuple		lightv;
	float		ldn;
	t_tuple		diffuse;
	t_tuple		specular;
	t_tuple		reflectv;
	float		rde;
	float		factor;
	t_tuple		result;
}				t_light_vars;

typedef struct s_light_params
{
	t_material	material;
	t_light		light;
	t_tuple		position;
	t_tuple		eyev;
	t_tuple		normalv;
	bool		in_shadow;
	t_tuple		ambient_color;
}				t_light_params;

t_light			point_light(t_tuple point, t_tuple color);
t_tuple			lighting(t_light_params params);

#endif
