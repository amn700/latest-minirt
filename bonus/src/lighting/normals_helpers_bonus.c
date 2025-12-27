/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normals_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"
#include <math.h>

/*
 * Get tangent and bitangent vectors for TBN matrix
 */
static void	get_tangent_bitangent(t_tuple normal, t_tuple *tangent, \
	t_tuple *bitangent)
{
	t_tuple	up;
	t_tuple	right;

	if (fabs(normal.y) > 0.999f)
		up = (t_tuple){0, 0, 1, 0};
	else
		up = (t_tuple){0, 1, 0, 0};
	right = vecs_cross_product(up, normal);
	*tangent = normalizing_vector(right);
	*bitangent = vecs_cross_product(normal, *tangent);
}

/*
 * Sample normal map and convert to tangent space normal
 */
static t_tuple	sample_normal_map(t_tex tex, float u, float v)
{
	t_tuple	color;
	t_tuple	normal;

	color = ft_sample_texture(tex, u, v);
	normal.x = 2.0f * color.x - 1.0f;
	normal.y = 2.0f * color.y - 1.0f;
	normal.z = 2.0f * color.z - 1.0f;
	normal.w = 0;
	return (normalizing_vector(normal));
}

/*
 * Transform tangent space normal to world space using TBN matrix
 */
static t_tuple	apply_tbn(t_tuple tangent_normal, t_tuple tangent, \
	t_tuple bitangent, t_tuple normal)
{
	t_tuple	result;

	result.x = tangent_normal.x * tangent.x + tangent_normal.y * bitangent.x \
		+ tangent_normal.z * normal.x;
	result.y = tangent_normal.x * tangent.y + tangent_normal.y * bitangent.y \
		+ tangent_normal.z * normal.y;
	result.z = tangent_normal.x * tangent.z + tangent_normal.y * bitangent.z \
		+ tangent_normal.z * normal.z;
	result.w = 0;
	return (normalizing_vector(result));
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

/*
 * Perturb normal using bump/normal map
 */
t_tuple	perturb_normal_bump(t_comps *comps, t_tuple normal)
{
	t_material	mat;
	t_tuple		tangent;
	t_tuple		bitangent;
	t_tuple		tangent_normal;

	mat = get_material(comps->obj);
	if (!mat.has_btex)
		return (normal);
	get_tangent_bitangent(normal, &tangent, &bitangent);
	tangent_normal = sample_normal_map(mat.btex, comps->u, comps->v);
	return (apply_tbn(tangent_normal, tangent, bitangent, normal));
}
