/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

/*
 * Sample a texture at UV coordinates
 */
t_tuple	ft_sample_texture(t_tex tex, float u, float v)
{
	int		x;
	int		y;
	t_color	pixel;
	t_tuple	color;

	if (!tex.pixels || tex.width == 0 || tex.height == 0)
		return ((t_tuple){0, 0, 0, 0});
	if (u < 0.0f)
		u = 0.0f;
	if (u > 1.0f)
		u = 1.0f;
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	x = (int)(u * (tex.width - 1));
	y = (int)(v * (tex.height - 1));
	pixel = tex.pixels[y][x];
	color.x = pixel.r / 255.0f;
	color.y = pixel.g / 255.0f;
	color.z = pixel.b / 255.0f;
	color.w = 0;
	return (color);
}

/*
 * Get material from object, or return default if invalid
 */
static t_material	get_material(t_object *obj)
{
	if (obj->type == OBJ_SPHERE)
		return (obj->shape.sp.material);
	else if (obj->type == OBJ_PLANE)
		return (obj->shape.pl.material);
	else if (obj->type == OBJ_CYLINDER)
		return (obj->shape.cy.material);
	return (material());
}

t_tuple	ft_tex_color_at(t_comps *comps)
{
	t_material	mat;
	t_tuple		color;

	mat = get_material(comps->obj);
	if (mat.has_tex)
	{
		color = ft_sample_texture(mat.tex, comps->u, comps->v);
		return (color);
	}
	return (mat.color);
}
