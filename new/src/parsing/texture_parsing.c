/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/27 00:00:00 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/ft_png.h"
#include <fcntl.h>
#include <unistd.h>

/*
 * Load a texture from a PNG file
 */
static bool	load_texture_file(char *path, t_tex *tex)
{
	t_png_img	img;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Cannot open texture file '%s'\n", path);
		return (false);
	}
	close(fd);
	img = ft_allocate_png_img(path);
	if (img.width == 0 || img.height == 0)
	{
		printf("Error: Failed to load texture from '%s'\n", path);
		return (false);
	}
	*tex = ft_png_img_to_tex(img);
	ft_free_png_img(&img);
	if (tex->width == 0 || tex->height == 0)
	{
		printf("Error: Failed to convert texture from '%s'\n", path);
		return (false);
	}
	return (true);
}

/*
 * Parse tex= parameter
 */
static bool	parse_tex_param(char *param, t_material *material)
{
	char	*path;

	if (ft_strncmp(param, "tex=", 4) != 0)
		return (false);
	path = param + 4;
	if (!path || *path == '\0')
	{
		printf("Error: Empty texture path in 'tex=' parameter\n");
		return (false);
	}
	if (!load_texture_file(path, &material->tex))
		return (false);
	material->has_tex = true;
	return (true);
}

/*
 * Parse btex= parameter
 */
static bool	parse_btex_param(char *param, t_material *material)
{
	char	*path;

	if (ft_strncmp(param, "btex=", 5) != 0)
		return (false);
	path = param + 5;
	if (!path || *path == '\0')
	{
		printf("Error: Empty texture path in 'btex=' parameter\n");
		return (false);
	}
	if (!load_texture_file(path, &material->btex))
		return (false);
	material->has_btex = true;
	return (true);
}

/*
 * Parse a texture parameter (tex= or btex=)
 */
bool	parse_texture_param(char *param, t_material *material)
{
	if (!param || !material)
		return (false);
	if (ft_strncmp(param, "tex=", 4) == 0)
		return (parse_tex_param(param, material));
	else if (ft_strncmp(param, "btex=", 5) == 0)
		return (parse_btex_param(param, material));
	return (false);
}

/*
 * Free texture resources
 */
void	free_material_textures(t_material *material)
{
	if (!material)
		return ;
	if (material->has_tex)
	{
		ft_free_tex(&material->tex);
		material->has_tex = false;
	}
	if (material->has_btex)
	{
		ft_free_tex(&material->btex);
		material->has_btex = false;
	}
}
