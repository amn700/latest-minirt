/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parsing_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"
#include "../../includes/texture_bonus.h"
#include "../MLX42/include/MLX42/MLX42.h"

/*
 * Parse texture path from parameter string
 * Format: "bump:path/to/file.png" or "normal:path/to/file.png"
 */
bool	parse_texture_path(char *param_str, char **out_path)
{
	char	*colon;
	char	*path;
	int		fd;

	if (!param_str || !out_path)
		return (false);
	colon = ft_strchr(param_str, ':');
	if (!colon || *(colon + 1) == '\0')
		return (false);
	path = ft_strdup(colon + 1);
	if (!path)
		return (false);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		free(path);
		return (false);
	}
	close(fd);
	*out_path = path;
	return (true);
}

/*
 * Parse texture strength from parameter string
 * Format: "bump_strength:0.5" or "normal_strength:1.0"
 */
bool	parse_texture_strength(char *param_str, float *out_strength)
{
	char	*colon;
	float	strength;

	if (!param_str || !out_strength)
		return (false);
	colon = ft_strchr(param_str, ':');
	if (!colon || *(colon + 1) == '\0')
		return (false);
	if (!f_field_validation(colon + 1))
		return (false);
	strength = ft_atof(colon + 1);
	if (strength < 0.0f || strength > 1.0f)
		return (false);
	*out_strength = strength;
	return (true);
}

/*
 * Load a texture and validate it
 */
static mlx_texture_t	*load_texture(char *path, mlx_t *mlx)
{
	mlx_texture_t	*texture;

	(void)mlx;
	texture = mlx_load_png(path);
	if (!texture)
	{
		printf("Error: Failed to load texture '%s'\n", path);
		return (NULL);
	}
	if (texture->width > 4096 || texture->height > 4096)
		printf("Warning: Texture '%s' is large (%ux%u), may impact performance\n",
			path, texture->width, texture->height);
	return (texture);
}

/*
 * Load material textures from scene file parameters
 */
bool	load_material_textures(t_material *material, char **params, mlx_t *mlx)
{
	int		i;
	char	*path;
	bool	saw_bump;
	bool	saw_normal;
	bool	saw_texture;

	if (!material || !params)
		return (false);
	i = 0;
	saw_bump = false;
	saw_normal = false;
	saw_texture = false;
	material->bump_strength = 0.5f;
	while (params[i])
	{
		if (ft_strncmp(params[i], "bump:", 5) == 0)
		{
			if (saw_bump)
			{
				printf("Error: Duplicate bump map parameter\n");
				return (false);
			}
			if (!parse_texture_path(params[i], &path))
			{
				printf("Error: Invalid bump map path in '%s'\n", params[i]);
				return (false);
			}
			material->bump_map = load_texture(path, mlx);
			free(path);
			if (!material->bump_map)
				return (false);
			material->has_bump_map = true;
			saw_bump = true;
		}
		else if (ft_strncmp(params[i], "bump_strength:", 14) == 0)
		{
			if (!parse_texture_strength(params[i], &material->bump_strength))
			{
				printf("Error: Invalid bump strength in '%s'\n", params[i]);
				return (false);
			}
		}
		else if (ft_strncmp(params[i], "normal:", 7) == 0)
		{
			if (saw_normal)
			{
				printf("Error: Duplicate normal map parameter\n");
				return (false);
			}
			if (!parse_texture_path(params[i], &path))
			{
				printf("Error: Invalid normal map path in '%s'\n", params[i]);
				return (false);
			}
			material->normal_map = load_texture(path, mlx);
			free(path);
			if (!material->normal_map)
				return (false);
			material->has_normal_map = true;
			saw_normal = true;
		}
		else if (ft_strncmp(params[i], "normal_strength:", 16) == 0)
		{
			if (!parse_texture_strength(params[i], &material->bump_strength))
			{
				printf("Error: Invalid normal strength in '%s'\n", params[i]);
				return (false);
			}
		}
		else if (ft_strncmp(params[i], "texture:", 8) == 0)
		{
			if (saw_texture)
			{
				printf("Error: Duplicate color texture parameter\n");
				return (false);
			}
			if (!parse_texture_path(params[i], &path))
			{
				printf("Error: Invalid texture path in '%s'\n", params[i]);
				return (false);
			}
			material->color_texture = load_texture(path, mlx);
			free(path);
			if (!material->color_texture)
				return (false);
			material->has_color_texture = true;
			saw_texture = true;
		}
		i++;
	}
	return (true);
}

/*
 * Free texture resources
 */
void	free_material_textures(t_material *material)
{
	if (!material)
		return ;
	if (material->bump_map)
	{
		mlx_delete_texture(material->bump_map);
		material->bump_map = NULL;
		material->has_bump_map = false;
	}
	if (material->normal_map)
	{
		mlx_delete_texture(material->normal_map);
		material->normal_map = NULL;
		material->has_normal_map = false;
	}
	if (material->color_texture)
	{
		mlx_delete_texture(material->color_texture);
		material->color_texture = NULL;
		material->has_color_texture = false;
	}
}
