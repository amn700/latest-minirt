/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 08:50:00 by amn               #+#    #+#             */
/*   Updated: 2025/11/23 06:59:56 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/miniRT.h"
#include <stdlib.h>

/**
 * Creates a single intersection object
 * @param t The distance along the ray where intersection occurs
 * @param shape Pointer to the shape that was intersected
 * @return Newly allocated intersection object, or NULL on failure
 */

t_inters *intersection(float t, t_shapes shape)
{
    t_inters *new_intersection;

    new_intersection = malloc(sizeof(t_inters));
    if (!new_intersection)
        return (NULL);
    
    // For now, store t values in xs[0] and xs[1] until we update the struct
    new_intersection->xs[0] = t;
    new_intersection->xs[1] = t; // Same value for consistency
    new_intersection->shape = shape;
    new_intersection->next = NULL;
    
    return (new_intersection);
}

/**
 * Creates an empty intersection list
 * @return NULL (empty list)
 */
t_inters *intersections_new(void)
{
    return (NULL);
}

/**
 * Adds intersection to list maintaining sorted order by t value
 * @param list Pointer to the list head
 * @param intersection The intersection to add
 */
void intersections_add(t_inters **list, t_inters *intersection)
{
    t_inters *current;
    t_inters *prev;

    if (!list || !intersection)
        return;

    // If list is empty or intersection should be first
    if (!*list || intersection->xs[0] < (*list)->xs[0])
    {
        intersection->next = *list;
        *list = intersection;
        return;
    }

    // Find correct position to maintain sorted order
    prev = *list;
    current = (*list)->next;
    
    while (current && current->xs[0] < intersection->xs[0])
    {
        prev = current;
        current = current->next;
    }
    
    // Insert intersection between prev and current
    intersection->next = current;
    prev->next = intersection;
}

/**
 * Gets the count of intersections in the list
 * @param list The intersection list
 * @return Number of intersections in the list
 */
int intersections_count(t_inters *list)
{
    int count;
    t_inters *current;

    count = 0;
    current = list;
    
    while (current)
    {
        count++;
        current = current->next;
    }
    
    return (count);
}

/**
 * Gets intersection at the specified index (0-based)
 * @param list The intersection list
 * @param index The index to retrieve (0-based)
 * @return Intersection at index, or NULL if index out of bounds
 */
t_inters *intersections_get(t_inters *list, int index)
{
    int current_index;
    t_inters *current;

    if (index < 0)
        return (NULL);
        
    current_index = 0;
    current = list;
    
    while (current && current_index < index)
    {
        current = current->next;
        current_index++;
    }
    
    return (current);
}

/**
 * Frees a single intersection object
 * @param intersection The intersection to free
 */
void intersection_free(t_inters *intersection)
{
    if (intersection)
        free(intersection);
}

/**
 * Frees an entire intersection list
 * @param list The list to free
 */
void intersections_free(t_inters *list)
{
    t_inters *current;
    t_inters *next;

    current = list;
    while (current)
    {
        next = current->next;
        intersection_free(current);
        current = next;
    }
}

/**
 * Simple sphere intersection function - just math, no objects
 * @param sp The sphere to intersect with
 * @param ray The ray to cast
 * @param inter Array to store the two intersection distances
 * @return true if intersections found, false otherwise
 */
bool intersect_sphere_simple(t_sphere sp, t_ray ray, float inter[2])
{
    t_tuple sp_to_ray;
    float a, b, c, discriminant;

    // Calculate discriminant
    sp_to_ray = substract_tuple(ray.origin, sp.center);
    a = vecs_dot_product(ray.direction, ray.direction);
    b = 2 * vecs_dot_product(ray.direction, sp_to_ray);
    c = vecs_dot_product(sp_to_ray, sp_to_ray) - 1; // Assuming unit sphere for now
    discriminant = (b * b) - (4 * a * c);
    
    // No intersections if discriminant is negative
    if (discriminant < 0)
        return (false);
    
    // Calculate intersection points
    inter[0] = (-b - sqrt(discriminant)) / (2 * a);
    inter[1] = (-b + sqrt(discriminant)) / (2 * a);
    
    return (true);
}

/**
 * Sphere intersection function that returns intersection objects
 * @param sp The sphere to intersect with
 * @param ray The ray to cast
 * @return List of intersection objects, or NULL if no intersections
 */
t_inters *intersect_sphere_objects(t_sphere sp, t_ray ray)
{
    float inter[2];
    t_inters *intersections_list;
    t_inters *intersection1;
    t_inters *intersection2;
    t_shapes shape;

    // Use simple function for math
    if (!intersect_sphere_simple(sp, ray, inter))
        return (NULL);

    // Create shape union with sphere
    shape.sp = sp;
    
    // Create intersection objects
    intersections_list = intersections_new();
    intersection1 = intersection(inter[0], shape);
    intersection2 = intersection(inter[1], shape);
    
    if (!intersection1 || !intersection2)
    {
        intersection_free(intersection1);
        intersection_free(intersection2);
        return (NULL);
    }
    
    // Add intersections to list (will be automatically sorted)
    intersections_add(&intersections_list, intersection1);
    intersections_add(&intersections_list, intersection2);
    
    return (intersections_list);
}

/**
 * Legacy function name - calls the object version for compatibility
 * @param sp The sphere to intersect with
 * @param ray The ray to cast
 * @return List of intersection objects, or NULL if no intersections
 */
// t_inters *intersect_sphere(t_sphere sp, t_ray ray)
// {
//     return intersect_sphere_objects(sp, ray);
// }
