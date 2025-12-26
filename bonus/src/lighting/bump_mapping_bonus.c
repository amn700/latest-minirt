/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/26 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"
#include "../../includes/texture_bonus.h"
#include "../MLX42/include/MLX42/MLX42.h"

/*
 * Sample grayscale height from bump map texture
 * Returns height value in range [0.0, 1.0]
 */
float	sample_bump_map(mlx_texture_t *texture, float u, float v)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	index;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	float		height;

	x = (uint32_t)(u * (texture->width - 1));
	y = (uint32_t)(v * (texture->height - 1));
	if (x >= texture->width)
		x = texture->width - 1;
	if (y >= texture->height)
		y = texture->height - 1;
	index = (y * texture->width + x) * 4;
	r = texture->pixels[index + 0];
	g = texture->pixels[index + 1];
	b = texture->pixels[index + 2];
	height = (r + g + b) / (3.0f * 255.0f);
	return (height);
}

/*
 * Sample RGB normal from normal map texture
 * Returns XYZ normal in range [-1, 1]
 */
t_tuple	sample_normal_map(mlx_texture_t *texture, float u, float v)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	index;
	t_tuple		normal;

	x = (uint32_t)(u * (texture->width - 1));
	y = (uint32_t)(v * (texture->height - 1));
	if (x >= texture->width)
		x = texture->width - 1;
	if (y >= texture->height)
		y = texture->height - 1;
	index = (y * texture->width + x) * 4;
	normal.x = texture->pixels[index + 0] / 127.5f - 1.0f;
	normal.y = texture->pixels[index + 1] / 127.5f - 1.0f;
	normal.z = texture->pixels[index + 2] / 127.5f - 1.0f;
	normal.w = 0;
	return (normalizing_vector(normal));
}

/*
 * Get UV coordinates based on object type
 */
static t_tuple	get_uv_coords(t_tuple local_point, t_obj_type obj_type)
{
	if (obj_type == OBJ_SPHERE)
		return (sphere_uv_mapping(local_point));
	else if (obj_type == OBJ_PLANE)
		return (plane_uv_mapping(local_point));
	else if (obj_type == OBJ_CYLINDER)
		return (cylinder_uv_mapping(local_point));
	else if (obj_type == OBJ_CONE)
		return (cone_uv_mapping(local_point));
	return ((t_tuple){0, 0, 0, 0});
}

/*
 * Compute tangent space basis vectors for sphere
 */
static void	get_tangent_basis_sphere(t_tuple local_point, t_tuple *tangent, 
		t_tuple *bitangent)
{
	float	theta;

	theta = atan2f(local_point.x, local_point.z);
	*tangent = (t_tuple){-sinf(theta), 0, cosf(theta), 0};
	*bitangent = vecs_cross_product(*tangent, local_point);
	*tangent = normalizing_vector(*tangent);
	*bitangent = normalizing_vector(*bitangent);
}

/*
 * Compute tangent space basis vectors for plane
 */
static void	get_tangent_basis_plane(t_tuple *tangent, t_tuple *bitangent)
{
	*tangent = (t_tuple){1, 0, 0, 0};
	*bitangent = (t_tuple){0, 0, 1, 0};
}

/*
 * Compute tangent space basis vectors for cylinder
 */
static void	get_tangent_basis_cylinder(t_tuple local_point, t_tuple *tangent,
		t_tuple *bitangent)
{
	float	theta;

	theta = atan2f(local_point.x, local_point.z);
	*tangent = (t_tuple){-sinf(theta), 0, cosf(theta), 0};
	*bitangent = (t_tuple){0, 1, 0, 0};
	*tangent = normalizing_vector(*tangent);
}

/*
 * Compute tangent space basis vectors
 */
static void	get_tangent_basis(t_tuple local_point, t_obj_type obj_type,
		t_tuple *tangent, t_tuple *bitangent)
{
	if (obj_type == OBJ_SPHERE)
		get_tangent_basis_sphere(local_point, tangent, bitangent);
	else if (obj_type == OBJ_PLANE)
		get_tangent_basis_plane(tangent, bitangent);
	else if (obj_type == OBJ_CYLINDER || obj_type == OBJ_CONE)
		get_tangent_basis_cylinder(local_point, tangent, bitangent);
}

/*
 * Perturb normal using bump map gradient
 */
t_tuple	perturb_normal_with_bump(t_material material, t_tuple normal,
		t_tuple local_point, t_obj_type obj_type)
{
	t_tuple		uv;
	float		h_center;
	float		h_u;
	float		h_v;
	float		du;
	float		dv;
	t_tuple		tangent;
	t_tuple		bitangent;
	t_tuple		perturbed;
	float		epsilon;

	if (!material.has_bump_map || material.bump_map == NULL)
		return (normal);
	epsilon = 0.001f;
	uv = get_uv_coords(local_point, obj_type);
	h_center = sample_bump_map(material.bump_map, uv.x, uv.y);
	h_u = sample_bump_map(material.bump_map, uv.x + epsilon, uv.y);
	h_v = sample_bump_map(material.bump_map, uv.x, uv.y + epsilon);
	du = (h_u - h_center) * material.bump_strength;
	dv = (h_v - h_center) * material.bump_strength;
	get_tangent_basis(local_point, obj_type, &tangent, &bitangent);
	perturbed = add_tuple(normal, tuple_scalar_mult(tangent, -du));
	perturbed = add_tuple(perturbed, tuple_scalar_mult(bitangent, -dv));
	return (normalizing_vector(perturbed));
}

/*
 * Apply normal map to surface
 */
t_tuple	apply_normal_map(t_material material, t_tuple normal,
		t_tuple local_point, t_obj_type obj_type)
{
	t_tuple		uv;
	t_tuple		tangent_normal;
	t_tuple		tangent;
	t_tuple		bitangent;
	t_tuple		world_normal;

	if (!material.has_normal_map || material.normal_map == NULL)
		return (normal);
	uv = get_uv_coords(local_point, obj_type);
	tangent_normal = sample_normal_map(material.normal_map, uv.x, uv.y);
	get_tangent_basis(local_point, obj_type, &tangent, &bitangent);
	world_normal = tuple_scalar_mult(tangent, tangent_normal.x);
	world_normal = add_tuple(world_normal,
			tuple_scalar_mult(bitangent, tangent_normal.y));
	world_normal = add_tuple(world_normal,
			tuple_scalar_mult(normal, tangent_normal.z));
	world_normal = normalizing_vector(world_normal);
	world_normal = add_tuple(tuple_scalar_mult(normal, 1.0f - material.bump_strength),
			tuple_scalar_mult(world_normal, material.bump_strength));
	return (normalizing_vector(world_normal));
}
