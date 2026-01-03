/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 10:00:39 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 10:01:15 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# ifndef EPSILON
#  define EPSILON 0.001f
# endif

# ifndef PLANE_EPSILON
#  define PLANE_EPSILON 0.1f
# endif

typedef struct s_screen
{
	double		x;
	double		y;
}				t_screen;

typedef struct s_computations
{
	float		t;
	t_object	*obj;
	t_tuple		point;
	t_tuple		eyev;
	t_tuple		normalv;
	t_tuple		over_point;
	bool		inside;
}				t_comps;

typedef struct s_world
{
	t_object	*objects;
	t_light		*lights;
	t_tuple		ambient_color;
}				t_world;

t_world			new_world(void);
t_inters		*intersect_world(t_world world, t_ray ray);
t_object		*new_object(t_obj_type type, t_shapes shape);
t_light			*new_light(t_tuple point, t_tuple color);
t_comps			*prepare_computations(t_inters *intersection, t_ray ray);
bool			f_equal(float a, float b);
bool			tuple_comparison(t_tuple a, t_tuple b);
t_tuple			add_tuple(t_tuple a, t_tuple b);
t_tuple			substract_tuple(t_tuple a, t_tuple b);
t_tuple			negate_tuple(t_tuple a);
t_tuple			tuple_scalar_mult(t_tuple a, float scalar);
t_tuple			tuple_scalar_divide(t_tuple a, float scalar);
t_tuple			normalizing_vector(t_tuple a);
float			vecs_dot_product(t_tuple va, t_tuple vb);
t_tuple			vecs_cross_product(t_tuple va, t_tuple vb);
t_tuple			Hadamard_product(t_tuple a, t_tuple b);
float			vector_magnitude(t_tuple v);
t_tuple			reflect(t_tuple in, t_tuple normal);
t_matrix		identity(void);
t_matrix		translation(float x, float y, float z);
t_matrix		scaling(float x, float y, float z);
t_matrix		rotation_x(float theta);
t_matrix		rotation_y(float theta);
t_matrix		rotation_z(float theta);
t_matrix		shearing(t_propo propotion);
double			to_radians(int degrees);
t_matrix		multiply_matrix(t_matrix a, t_matrix b);
t_tuple			multiply_matrix_by_tuple(t_matrix m, t_tuple t);
t_matrix		transposing_matrix(t_matrix m);
float			determinant(t_matrix m);
t_matrix		submatrix(t_matrix m, int row, int col);
t_matrix		minor(t_matrix m, int row, int col);
float			cofactor(t_matrix m, int row, int col);
bool			is_invertible(t_matrix m);
t_matrix		inverse_matrix(t_matrix m);
t_ray			transform_ray(t_ray ray, t_matrix m);
t_sphere		sp_transform(t_matrix m, t_sphere sp);
t_tuple			sphere_normal_at(t_sphere sp, t_tuple p);
t_material		material(void);
t_sphere		sphere(void);
float			hit(t_inters *list);
bool			intersect_sphere(t_sphere *sp, t_ray ray,
					t_inters **intersections, t_object *obj);
bool			mlx_image_to_ppm(mlx_image_t *img, char *string,
					unsigned int range);
int				convert_color(t_tuple c);
t_tuple			point(int pixel_x, int pixel_y, int canvas_width);

void			sort_intersections(t_inters **lst);
t_tuple			position(t_ray ray, float distance);
t_inters		*intersection(float t, t_shapes shape);
t_inters		*intersections_new(void);
void			intersections_add(t_inters **list, t_inters *intersection);
int				intersections_count(t_inters *list);
t_inters		*intersections_get(t_inters *list, int index);
void			intersection_free(t_inters *intersection);
void			intersections_free(t_inters *list);

#endif