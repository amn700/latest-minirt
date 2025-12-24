/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 09:30:52 by amn               #+#    #+#             */
/*   Updated: 2025/12/09 07:39:11 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/miniRT_bonus.h"

#include "../../includes/miniRT_bonus.h"


t_matrix	identity(void)
{
	t_matrix identity_matrix;
	identity_matrix.size = 4;

	// Create identity matrix
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			identity_matrix.mtrx[row][col] = 0;
			if (col == row)
				identity_matrix.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	return identity_matrix;
}


void	ft_add_intersection(t_inters **lst, t_inters *new)
{
	t_inters	*ptr;

	if (!new || !lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}

t_inters	*new_intersection(float t, t_object *obj)
{
	t_inters *new = malloc(sizeof(t_inters));
	if (!new)
		return NULL;
	new->object = obj;
	new->t = t;
	new->next = NULL;
	return new;
}


void	sort_intersections(t_inters **lst)
{
	t_inters	*i1;
	t_inters	*i2;
	float		temp_t;
	t_object	*temp_obj;

	if (!lst || !*lst)
		return ;
	i1 = *lst;
	while (i1)
	{
		i2 = i1->next;
		while (i2)
		{
			if (i1->t > i2->t)
			{
				temp_t = i1->t;
				temp_obj = i1->object;
				i1->t = i2->t;
				i1->object = i2->object;
				i2->t = temp_t;
				i2->object = temp_obj;
			}
			i2 = i2->next;
		}
		i1 = i1->next;
	}
}

bool    intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections)
{
	t_tuple sp_to_ray = substract_tuple(ray.origin, sp->center);
	float a = vecs_dot_product(ray.direction, ray.direction);
	float b = 2 * vecs_dot_product(ray.direction, sp_to_ray);
	float c = vecs_dot_product(sp_to_ray, sp_to_ray) - 1;
	float descrimenent = ((b * b) - (4 * a) * c);
	
	if (descrimenent < 0)
		return false;
	float *xs = malloc(sizeof(float) * 2);
	xs[0] = (-b - sqrt(descrimenent)) / (2 * a);
	xs[1] = (-b + sqrt(descrimenent)) / (2 * a);
	
	// Create separate object structures for each intersection to avoid double free
	t_object *obj1 = malloc(sizeof(t_object));
	t_object *obj2 = malloc(sizeof(t_object));
	if (!obj1 || !obj2)
	{
		free(obj1);
		free(obj2);
		free(xs);
		return false;
	}
	
	obj1->type = OBJ_SPHERE;
	obj1->shape.sp = *sp;  // Copy the sphere data
	obj1->next = NULL;
	
	obj2->type = OBJ_SPHERE;
	obj2->shape.sp = *sp;  // Copy the sphere data
	obj2->next = NULL;
	
	ft_add_intersection(intersections, new_intersection(xs[0], obj1));
	ft_add_intersection(intersections, new_intersection(xs[1], obj2));
	sort_intersections(intersections);
	free(xs);
	return true;
} 

float	hit(t_inters *list)
{
	t_inters	*current = list;
	float		min_positive = INFINITY;

	while (current)
	{
		//isnt fequal function better here?
		if (current->t >= 0 && current->t < min_positive)
			min_positive = current->t;
		current = current->next;
	}
	if (min_positive == INFINITY)
		return -1;
	return min_positive;
}

t_tuple	position(t_ray ray, float distance)
{
	return add_tuple(ray.origin, tuple_scalar_mult(ray.direction, distance));
}

t_material material(void)
{
    return (t_material){
		.color = {1, 1, 1, 0},
        .ambient = 0.1,
        .diffuse = 0.9,
        .specular = 0.9,
		.reflective = 0.5,
        .shininess = 200.0,
		.transparency = 0.0,
		.refract_index = 1.0
    };
}

t_sphere	sphere(void)
{
	return (t_sphere){
		.trans = identity(),
		.diam = 1,
		.center = (t_tuple){0,0,0,1},
		.material = material()};
}

t_sphere	glass_sphere(void)
{
	return (t_sphere){
		.trans = identity(),
		.diam = 1,
		.center = (t_tuple){0,0,0,1},
		.material = material(),
		.material.transparency = 1.0,
		.material.refract_index = 1.5
	};
}

t_sphere	sp_transform(t_matrix m, t_sphere sp)
{
	t_tuple new_center = multiply_matrix_by_tuple(m, sp.center);
	return (t_sphere){.diam = sp.diam, .center = new_center, .material = sp.material, .trans = identity()};
}

t_ray transform_ray(t_ray ray, t_matrix m)
{
	t_tuple new_origin = multiply_matrix_by_tuple(m, ray.origin);
	t_tuple new_direction = multiply_matrix_by_tuple(m, ray.direction);
	return (t_ray){new_origin, new_direction};
}

// lighting(t_tuple point, t_material material, t_light light, )
// {
		
// }

// int main ()
// {
	// t_sphere sp = sphere();
	// sp = sp_transform(identity(), sp);
	// printf("Sphere center after identity transform: %.2f, %.2f, %.2f, %.2f\n", sp.center.x, sp.center.y, sp.center.z, sp.center.w);

	// t_sphere sp2 = sphere();
	// t_matrix trans = translation(2,3,4);
	// sp2 = sp_transform(trans, sp2);

	// printf("Sphere center after translation: %.2f, %.2f, %.2f, %.2f\n", sp2.center.x, sp2.center.y, sp2.center.z, sp2.center.w);
////
	// t_ray ray = {(t_tuple){0,0,-5,1}, (t_tuple){0,0,1,0}};
	// t_sphere sp0 = sphere();
	// ray = trasform_ray(ray, inverse_matrix(scaling(2,2,2)));
	// t_inters *intersections = NULL;
	// intersect(sp0, ray, &intersections);
	
	// t_inters *current = intersections;
	// while (current)
	// {
	// 	printf("Intersection at t=%.2f with object at center: %.2f, %.2f, %.2f, %.2f\n", current->t, current->object->shape.sp.center.x, current->object->shape.sp.center.y, current->object->shape.sp.center.z, current->object->shape.sp.center.w);
	// 	current = current->next;
	// }
////
	// t_ray ray = {(t_tuple){0,0,-5,1}, (t_tuple){0,0,1,0}};
	// t_sphere sp1 = sphere();
	// t_inters *intersections = NULL;
	// ray = trasform_ray(ray, inverse_matrix(translation(5,0,0)));
	// intersect(sp1, ray, &intersections);
	// printf("Intersections after moving sphere:\n");
	// t_inters *current = intersections;
	// while (current)
	// {
	// 	printf("Intersection at t=%.2f with object at center: %.2f, %.2f, %.2f, %.2f\n", current->t, current->object->shape.sp.center.x, current->object->shape.sp.center.y, current->object->shape.sp.center.z, current->object->shape.sp.center.w);
	// 	current = current->next;
	// }

	// t_ray r1 = {(t_tuple){1,2,3,1}, (t_tuple){0,1,0,0}};
	// t_matrix m = translation(3,4,5);
	// t_ray r2 = trasform_ray(r1, m);
	// printf("r2 origin: %.2f, %.2f, %.2f, %.2f\n", r2.origin.x, r2.origin.y, r2.origin.z, r2.origin.w);
	// printf("r2 direction: %.2f, %.2f, %.2f, %.2f\n", r2.direction.x, r2.direction.y, r2.direction.z, r2.direction.w);
	
	//testing scaling
	// t_matrix s = scaling(2,3,4);
	// t_ray r3 = trasform_ray(r1, s);
	// printf("r3 origin: %.2f, %.2f, %.2f, %.2f\n", r3.origin.x, r3.origin.y, r3.origin.z, r3.origin.w);
	// printf("r3 direction: %.2f, %.2f, %.2f, %.2f\n", r3.direction.x, r3.direction.y, r3.direction.z, r3.direction.w);
// }

// int main ()
// {
// 	t_ray ray = {(t_tuple){0,0,-5,1}, (t_tuple){0,0,1,0}};
// 	t_inters *intersections = NULL;
// 	t_object obj;
// 	obj.type = OBJ_SPHERE;
// 	obj.shape.sp = sphere();
// 	if (intersect(obj.shape.sp, ray, &intersections))
// 		printf("Found Intersection\n");
// 	else
// 		printf("No intersections\n");

// 	t_object obj2;
// 	obj2.type = OBJ_SPHERE;
// 	obj2.shape.sp = sphere();
// 	obj2.shape.sp.center = (t_tuple){0,0,3,1};
// 	if (intersect(obj2.shape.sp, ray, &intersections))
// 		printf("Found Intersection\n");
// 	else
// 		printf("No intersections\n");

// 	t_inters *current = intersections;
// 	while (current)
// 	{
// 		printf("Intersection at t=%.2f\n", current->t);
// 		current = current->next;
// 	}
// 	printf("Hit at t=%.2f\n", hit(intersections));
// 	return 0;
// }

// int main ()
// {
//     t_ray   ray = {(t_tuple){0,0,5,1}, (t_tuple){0,0,1,0}};
//     float xs[2];
//     if (!intersect(sphere(), ray, xs))
//         printf("no intersection found\n");
//     else
//         printf("%.2f %.2f\n",xs[0], xs[1]);
//     // t_tuple pos1 =  position(ray, 0);
//     // printf("%.2f, %.2f, %.2f, %.2f\n", pos1.x, pos1.y, pos1.z, pos1.w);

//     // t_tuple pos2 =  position(ray, 1);
//     // printf("%.2f, %.2f, %.2f, %.2f\n", pos2.x,pos2.y,pos2.z,pos2.w);
	
//     // t_tuple pos3 =  position(ray, -1);
//     // printf("%.2f, %.2f, %.2f, %.2f\n", pos3.x, pos3.y, pos3.z, pos3.w);
	
//     // t_tuple pos4 =  position(ray, 2.5);
//     // printf("%.2f, %.2f, %.2f, %.2f\n", pos4.x, pos4.y, pos4.z, pos4.w);
// }
