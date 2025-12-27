/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/22 22:38:24 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_BONUS_H
# define TYPES_BONUS_H

# include <stdint.h>
# include <stdbool.h>

typedef struct s_matrix
{
	float	mtrx[4][4];
	int		size;
}	t_matrix;

typedef struct s_propo
{
	float	xy;
	float	xz;
	float	yx;
	float	yz;
	float	zx;
	float	zy;
}	t_propo;

typedef struct s_tuple
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_tuple;

typedef struct s_ray
{
	t_tuple	origin;
	t_tuple	direction;
}	t_ray;

typedef enum e_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER,
	OBJ_CONE
}	t_obj_type;

typedef struct s_pattern
{
    t_tuple		a;
    t_tuple		b;
	t_tuple     (*at)(struct s_pattern pattern, t_tuple point);
	t_matrix    transform;
	t_matrix    inv_transform;
	bool        has_transform;
}           	t_pattern;

typedef struct mlx_texture mlx_texture_t;

typedef struct s_material
{
	t_tuple	color;
	float	ambient;
	float	diffuse;
	float	specular;
	float	shininess;
	float	reflective;
	float	transparency;
	float	refract_index;
	t_pattern	pattern;
	mlx_texture_t	*bump_map;
	mlx_texture_t	*normal_map;
	float	bump_strength;
	bool	has_bump_map;
	bool	has_normal_map;
}	t_material;

typedef struct s_sphere
{
	t_tuple		center;
	float		diam;
	t_matrix	trans;
	t_material	material;
}	t_sphere;

typedef struct s_plane
{
	t_tuple		point;
	t_tuple		normal;
	t_matrix	trans;
	t_material	material;
}	t_plane;

typedef struct s_cylinder
{
	t_tuple		center;
	t_tuple		axis;
	float		diameter;
	float		height;
	t_matrix	trans;
	t_material	material;
	float		minimum;
	float		maximum;
	bool		closed;
}	t_cylinder;

typedef struct s_cone
{
	t_tuple		center;
	t_tuple		axis;
	float		diameter;
	float		height;
	float		angle;
	t_matrix	trans;
	t_material	material;
	float		minimum;
	float		maximum;
	bool		closed;
}	t_cone;

typedef union u_shape
{
	t_sphere	sp;
	t_plane		pl;
	t_cylinder	cy;
	t_cone		co;
}	t_shapes;

typedef struct s_object
{
	t_obj_type		type;
	t_shapes		shape;
	struct s_object	*next;
}	t_object;

typedef struct s_light
{
	t_tuple			origin;
	t_tuple			color;
	float			brightness;
	struct s_light	*next;
}	t_light;

typedef struct s_inters
{
	float				t;
	t_object			*object;
	struct s_inters		*next;
}	t_inters;

typedef struct s_ambient_light
{
	float	l_ratio;
	t_tuple	color;
}	t_ambient_light;

typedef struct s_camera
{
	double		pixel_size;
	double		h_view;
	double		v_view;
	double		fov;
	t_matrix	transform;
	double		half_width;
	double		half_height;
	t_tuple		from;
	t_tuple		to;
	t_tuple		up;
}	t_camera;

typedef struct s_computations
{
	float		t;
	t_object	*obj;
	t_tuple		point;
	t_tuple		eyev;
	t_tuple		normalv;
	t_tuple		reflectv;
	t_tuple		over_point;
	t_tuple		under_point;
	bool		inside;
	float		n1;
	float		n2;
}	t_comps;

typedef struct s_world
{
	t_object	*objects;
	t_light		*lights;
	t_tuple		ambient_color;
}	t_world;

// typedef struct s_screen
// {
// 	double	x;
// 	double	y;
// }	t_screen;

#endif
