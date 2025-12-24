/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/11/23 06:59:37 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Temporary type definitions for testing
typedef struct s_tuple
{
    float   x;
    float   y;
    float   z;
    float   w;
} t_tuple;

typedef struct s_sphere
{
    t_tuple center;
    float   diam;
    t_tuple color;
} t_sphere;

typedef union u_shape
{
    t_sphere    sp;
} t_shapes;

typedef struct s_ray
{
    t_tuple origin;
    t_tuple direction;
} t_ray;

typedef struct s_inters
{
    float               xs[2];      // Temporary - will be changed to single t later
    t_shapes            shape;
    struct s_inters     *next;
} t_inters;

// Helper functions for testing
t_tuple substract_tuple(t_tuple a, t_tuple b)
{
    return (t_tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

float vecs_dot_product(t_tuple va, t_tuple vb)
{
    return (va.x * vb.x + va.y * vb.y + va.z * vb.z);
}

// Include our intersection functions (copy-paste for testing)
t_inters *intersection(float t, t_shapes shape)
{
    t_inters *new_intersection;

    new_intersection = malloc(sizeof(t_inters));
    if (!new_intersection)
        return (NULL);
    
    new_intersection->xs[0] = t;
    new_intersection->xs[1] = t;
    new_intersection->shape = shape;
    new_intersection->next = NULL;
    
    return (new_intersection);
}

t_inters *intersections_new(void)
{
    return (NULL);
}

void intersections_add(t_inters **list, t_inters *intersection)
{
    t_inters *current;
    t_inters *prev;

    if (!list || !intersection)
        return;

    if (!*list || intersection->xs[0] < (*list)->xs[0])
    {
        intersection->next = *list;
        *list = intersection;
        return;
    }

    prev = *list;
    current = (*list)->next;
    
    while (current && current->xs[0] < intersection->xs[0])
    {
        prev = current;
        current = current->next;
    }
    
    intersection->next = current;
    prev->next = intersection;
}

int intersections_count(t_inters *list)
{
    int count = 0;
    t_inters *current = list;
    
    while (current)
    {
        count++;
        current = current->next;
    }
    
    return (count);
}

t_inters *intersections_get(t_inters *list, int index)
{
    int current_index = 0;
    t_inters *current = list;

    if (index < 0)
        return (NULL);
    
    while (current && current_index < index)
    {
        current = current->next;
        current_index++;
    }
    
    return (current);
}

void intersection_free(t_inters *intersection)
{
    if (intersection)
        free(intersection);
}

void intersections_free(t_inters *list)
{
    t_inters *current = list;
    t_inters *next;

    while (current)
    {
        next = current->next;
        intersection_free(current);
        current = next;
    }
}

// t_inters *intersect_sphere(t_sphere sp, t_ray ray)
// {
//     t_tuple sp_to_ray;
//     float a, b, c, discriminant;
//     float t1, t2;
//     t_inters *intersections_list;
//     t_inters *intersection1;
//     t_inters *intersection2;
//     t_shapes shape;

//     shape.sp = sp;

//     sp_to_ray = substract_tuple(ray.origin, sp.center);
//     a = vecs_dot_product(ray.direction, ray.direction);
//     b = 2 * vecs_dot_product(ray.direction, sp_to_ray);
//     c = vecs_dot_product(sp_to_ray, sp_to_ray) - 1;
//     discriminant = (b * b) - (4 * a * c);
    
//     if (discriminant < 0)
//         return (NULL);
    
//     t1 = (-b - sqrt(discriminant)) / (2 * a);
//     t2 = (-b + sqrt(discriminant)) / (2 * a);
    
//     intersections_list = intersections_new();
//     intersection1 = intersection(t1, shape);
//     intersection2 = intersection(t2, shape);
    
//     if (!intersection1 || !intersection2)
//     {
//         intersection_free(intersection1);
//         intersection_free(intersection2);
//         return (NULL);
//     }
    
//     intersections_add(&intersections_list, intersection1);
//     intersections_add(&intersections_list, intersection2);
    
//     return (intersections_list);
// }

// Test functions based on "The Ray Tracer Challenge"
void test_ray_sphere_intersection()
{
    printf("=== Testing Ray-Sphere Intersection ===\n");
    
    // Test 1: Ray intersects sphere at two points
    t_ray ray = {(t_tuple){0,0,-5,1}, (t_tuple){0,0,1,0}};
    t_sphere sp = {(t_tuple){0,0,0,1}, 1, (t_tuple){1,0,0,0}};
    
    t_inters *xs = intersect_sphere(sp, ray);
    
    if (xs)
    {
        printf("Test 1 - Intersection count: %d\n", intersections_count(xs));
        printf("Test 1 - t1: %.2f, t2: %.2f\n", 
               intersections_get(xs, 0)->xs[0], 
               intersections_get(xs, 1)->xs[0]);
        printf("Expected: t1: 4.00, t2: 6.00\n");
        intersections_free(xs);
    }
    else
    {
        printf("Test 1 - No intersections found (ERROR)\n");
    }
    
    // Test 2: Ray tangent to sphere
    t_ray ray2 = {(t_tuple){0,1,-5,1}, (t_tuple){0,0,1,0}};
    xs = intersect_sphere(sp, ray2);
    
    if (xs)
    {
        printf("Test 2 - Tangent intersection: %.2f, %.2f\n",
               intersections_get(xs, 0)->xs[0],
               intersections_get(xs, 1)->xs[0]);
        printf("Expected: 5.00, 5.00\n");
        intersections_free(xs);
    }
    else
    {
        printf("Test 2 - No intersections found (ERROR)\n");
    }
    
    // Test 3: Ray misses sphere
    t_ray ray3 = {(t_tuple){0,2,-5,1}, (t_tuple){0,0,1,0}};
    xs = intersect_sphere(sp, ray3);
    
    if (xs)
    {
        printf("Test 3 - Miss test failed - found intersections (ERROR)\n");
        intersections_free(xs);
    }
    else
    {
        printf("Test 3 - Ray misses sphere (CORRECT)\n");
    }
}

int main()
{
    test_ray_sphere_intersection();
    return 0;
}