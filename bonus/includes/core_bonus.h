/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_BONUS_H
# define CORE_BONUS_H

# include "types_bonus.h"

t_world		new_world(void);
t_object	*new_object(t_obj_type type, t_shapes shape);
void		ft_add_object(t_object **lst, t_object *new);
t_light		*new_light(t_tuple point, t_tuple color);
void		ft_add_light(t_light **lst, t_light *new);
t_inters	*intersect_world(t_world world, t_ray ray);
bool		intersect_world_shadow(t_world world, t_ray ray, float max_distance);
t_comps		*prepare_computations(t_inters *intersection, t_ray ray, \
			t_inters *intersections);
t_camera	camera(double hsize, double vsize, double field_of_view);
t_matrix	view_transformation(t_tuple from, t_tuple to, t_tuple up);
t_ray		ray_for_pixel(t_camera cam, double px, double py);
bool		is_shadowed(t_world world, t_tuple point, t_light *light);
t_tuple		shade_hit(t_world world, t_comps *comp, int depth);
t_tuple		color_at(t_world world, t_ray ray, int depth);
t_tuple		reflected_color(t_world world, t_comps *comps, int depth);
t_tuple		refracted_color(t_world world, t_comps *comps, int depth);
void		compute_n1_n2(t_inters *hit, t_inters *xs, t_comps *comps);

# define MAX_REFLECTION_DEPTH 5
t_sphere	glass_sphere(void);
#endif
