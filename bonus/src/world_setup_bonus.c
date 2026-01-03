#include "miniRT_bonus.h"

// typedef struct s_world
// {
// 	t_object    *objects;
// 	t_light     *lights;
// }   t_world;
bool    intersect_cylinder(t_cylinder *cyl, t_ray ray, t_inters **intersections);
bool    intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections);
t_world new_world(void)
{ 
	t_world world;

	world.objects = NULL;
	world.lights = NULL;
	world.ambient_color = (t_tuple){1, 1, 1, 0};  // Default white ambient
	return world;
}

void	ft_add_object(t_object **lst, t_object *new)
{
	t_object	*ptr;

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

void	ft_add_light(t_light **lst, t_light *new)
{
	t_light	*ptr;

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

t_light *new_light(t_tuple point, t_tuple color)
{
	t_light *light = malloc (sizeof(t_light));
	if (!light)
		return NULL;
	light->origin = point;
	light->color = color;
	light->next = NULL;
	return light;
}

t_object *new_object(t_obj_type type, t_shapes shape)
{
	t_object *obj = malloc (sizeof(t_object));
	if (!obj)
		return NULL;
	obj->type = type;
	obj->shape = shape;
	obj->next = NULL;
	return obj;
}


t_inters *intersect_world(t_world world, t_ray ray)
{
	t_object *obj = world.objects;
	t_inters *all_intersections = NULL;

	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			t_ray transformed_ray = transform_ray(ray, obj->shape.sp.trans_inv);
			intersect_sphere(&obj->shape.sp, transformed_ray, &all_intersections);
		}
		else if (obj->type == OBJ_PLANE)
		{
			t_ray transformed_ray = transform_ray(ray, obj->shape.pl.trans_inv);
			intersect_plane(&obj->shape.pl, transformed_ray, &all_intersections);
		}
		else if (obj->type == OBJ_CYLINDER)
		{
			t_ray transformed_ray = transform_ray(ray, obj->shape.cy.trans_inv);
			intersect_cylinder(&obj->shape.cy, transformed_ray, &all_intersections);
		}
		obj = obj->next;
	}
	return all_intersections;
}
