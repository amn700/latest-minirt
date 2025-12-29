/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/28 16:01:41 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_BONUS_H
# define TEXTURE_BONUS_H

# include "types_bonus.h"
# ifndef MLX42_TYPES_DECL
#  define MLX42_TYPES_DECL
typedef struct mlx mlx_t;
typedef struct mlx_image mlx_image_t;
typedef struct mlx_texture mlx_texture_t;
# endif

// Texture Sampling
float		sample_bump_map(mlx_texture_t *texture, float u, float v);
t_tuple		sample_normal_map(mlx_texture_t *texture, float u, float v);
t_tuple		sample_color_from_texture(mlx_texture_t *texture, float u, float v);

// Normal Perturbation
t_tuple		perturb_normal_with_bump(t_material material, t_tuple normal, 
				t_tuple local_point, t_obj_type obj_type);
t_tuple		apply_normal_map(t_material material, t_tuple normal, 
				t_tuple local_point, t_obj_type obj_type);

// Texture Parsing
bool		parse_texture_path(char *param_str, char **out_path);
bool		parse_texture_strength(char *param_str, float *out_strength);
bool		load_material_textures(t_material *material, char **params, 
				mlx_t *mlx);
void		free_material_textures(t_material *material);

#endif
