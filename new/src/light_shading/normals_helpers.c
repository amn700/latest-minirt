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

#include "../../includes/types.h"
#include "../../includes/math_ops.h"
#include <math.h>

// External function from texture_helpers.c
t_tuple	ft_get_uv_coords(t_tuple local_point, t_obj_type obj_type);
t_tuple	ft_uv_pattern_at(t_tex texture, float u, float v);

// Compute tangent basis for sphere
static void	ft_tangent_basis_sphere(t_tuple local_point, t_tuple *tangent,
			t_tuple *bitangent)
{
	float	theta;

	theta = atan2f(local_point.x, local_point.z);
	*tangent = (t_tuple){-sinf(theta), 0, cosf(theta), 0};
	*bitangent = vecs_cross_product(*tangent, local_point);
	*tangent = normalizing_vector(*tangent);
	*bitangent = normalizing_vector(*bitangent);
}

// Compute tangent basis for plane
static void	ft_tangent_basis_plane(t_tuple *tangent, t_tuple *bitangent)
{
	*tangent = (t_tuple){1, 0, 0, 0};
	*bitangent = (t_tuple){0, 0, 1, 0};
}

// Compute tangent basis for cylinder
static void	ft_tangent_basis_cylinder(t_tuple local_point, t_tuple *tangent,
			t_tuple *bitangent)
{
	float	theta;

	theta = atan2f(local_point.x, local_point.z);
	*tangent = (t_tuple){-sinf(theta), 0, cosf(theta), 0};
	*bitangent = (t_tuple){0, 1, 0, 0};
	*tangent = normalizing_vector(*tangent);
}

// Get tangent basis based on object type
static void	ft_get_tangent_basis(t_tuple local_point, t_obj_type obj_type,
			t_tuple *tangent, t_tuple *bitangent)
{
	if (obj_type == OBJ_SPHERE)
		ft_tangent_basis_sphere(local_point, tangent, bitangent);
	else if (obj_type == OBJ_PLANE)
		ft_tangent_basis_plane(tangent, bitangent);
	else if (obj_type == OBJ_CYLINDER)
		ft_tangent_basis_cylinder(local_point, tangent, bitangent);
}

// Sample normal map and convert RGB to tangent-space normal
static t_tuple	ft_sample_normal_map(t_tex texture, float u, float v)
{
	t_tuple	rgb;
	t_tuple	normal;

	rgb = ft_uv_pattern_at(texture, u, v);
	normal.x = rgb.x * 2.0f - 1.0f;
	normal.y = rgb.y * 2.0f - 1.0f;
	normal.z = rgb.z * 2.0f - 1.0f;
	normal.w = 0;
	return (normalizing_vector(normal));
}

// Perturb normal using bump/normal map
t_tuple	ft_perturb_normal_with_btex(t_material material, t_tuple normal,
		t_tuple local_point, t_obj_type obj_type)
{
	t_tuple	uv;
	t_tuple	tangent_normal;
	t_tuple	tangent;
	t_tuple	bitangent;
	t_tuple	world_normal;

	if (!material.btex.pixels || material.btex.width == 0
		|| material.btex.height == 0)
		return (normal);
	uv = ft_get_uv_coords(local_point, obj_type);
	tangent_normal = ft_sample_normal_map(material.btex, uv.x, uv.y);
	ft_get_tangent_basis(local_point, obj_type, &tangent, &bitangent);
	world_normal = tuple_scalar_mult(tangent, tangent_normal.x);
	world_normal = add_tuple(world_normal,
			tuple_scalar_mult(bitangent, tangent_normal.y));
	world_normal = add_tuple(world_normal,
			tuple_scalar_mult(normal, tangent_normal.z));
	return (normalizing_vector(world_normal));
}
