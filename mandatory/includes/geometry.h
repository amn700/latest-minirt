/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:03:18 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H

# include "types.h"

# define EPSILON 0.001f
# define PLANE_EPSILON 0.1f

t_sphere	sphere(void);
t_material	material(void);
t_sphere	sp_transform(t_matrix m, t_sphere sp);
t_plane		plane(void);
t_cylinder	cylinder(void);
t_tuple		sphere_normal_at(t_sphere sp, t_tuple p);
t_tuple		plane_normal_at(t_plane pl);
t_tuple		cylinder_normal_at(t_cylinder cyl, t_tuple point);
bool		intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections,
				t_object *obj);
bool		intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections,
				t_object *obj);
bool		intersect_cylinder(t_cylinder *cyl, t_ray ray,
				t_inters **intersections, t_object *obj);
t_inters	*new_intersection(float t, t_object *obj);
void		ft_add_intersection(t_inters **lst, t_inters *new);
void		sort_intersections(t_inters **lst);
float		hit(t_inters *list);
t_tuple		position(t_ray ray, float distance);
t_ray		transform_ray(t_ray ray, t_matrix m);

#endif
