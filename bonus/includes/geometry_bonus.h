/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 09:01:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_BONUS_H
# define GEOMETRY_BONUS_H

# include "types_bonus.h"

# define EPSILON 0.0001f
# define CONE_EPSILON 0.0001f
# define PLANE_EPSILON 0.0001f

// Scale-adaptive epsilon: grows gently with distance
// For t=1, returns ~0.0002; for t=100, returns ~0.01; for t=1000, returns ~0.1
static inline float	adaptive_epsilon(float t)
{
	float	abs_t;
	float	result;

	abs_t = t;
	if (t < 0)
		abs_t = -t;
	result = EPSILON * (1.0f + abs_t * 0.001f);
	if (result > 0.5f)
		result = 0.5f;
	return (result);
}

t_sphere	sphere(void);
t_material	material(void);
t_sphere	sp_transform(t_matrix m, t_sphere sp);
t_plane		plane(void);
t_cylinder	cylinder(void);
t_cone		cone(void);
t_tuple		sphere_normal_at(t_sphere sp, t_tuple p);
t_tuple		plane_normal_at(t_plane pl);
t_tuple		cylinder_normal_at(t_cylinder cyl, t_tuple point);
t_tuple		cone_normal_at(t_cone cone, t_tuple point);
t_tuple		cone_local_normal_at(t_cone cone, t_tuple object_point);
bool		intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections, \
			t_object *obj);
bool		intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections, \
			t_object *obj);
bool		intersect_cylinder(t_cylinder *cyl, t_ray ray, \
			t_inters **intersections, t_object *obj);
bool		intersect_cone(t_cone *cone, t_ray ray, \
			t_inters **intersections, t_object *obj);
t_inters	*new_intersection(float t, t_object *obj);
void		ft_add_intersection(t_inters **lst, t_inters *new);
void		sort_intersections(t_inters **lst);
float		hit(t_inters *list);
t_tuple		position(t_ray ray, float distance);
t_ray		transform_ray(t_ray ray, t_matrix m);

// UV Mapping
t_tuple		sphere_uv_mapping(t_tuple local_point);
t_tuple		plane_uv_mapping(t_tuple local_point);
t_tuple		cylinder_uv_mapping(t_tuple local_point);
t_tuple		cone_uv_mapping(t_tuple local_point);

#endif
