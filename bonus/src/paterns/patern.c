/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patern.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:16:52 by amn               #+#    #+#             */
/*   Updated: 2025/12/19 06:39:15 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/types_bonus.h"
#include "../../includes/math_ops_bonus.h"
#include <math.h>

// Forward declarations
t_tuple stripe_at(t_pattern pattern, t_tuple point);
t_tuple ring_at(t_pattern pattern, t_tuple point);
t_tuple checkers_at(t_pattern pattern, t_tuple point);

t_tuple black(void)
{
    return ((t_tuple) {0,0,0,0});
}

t_tuple white(void)
{
    return ((t_tuple) {1,1,1,0});
}

t_pattern stripe_patern(t_tuple a, t_tuple b)
{
    t_pattern p;

    p.a = a;
    p.b = b;
    p.at = stripe_at;
    p.transform = identity();
    p.inv_transform = identity();
    p.has_transform = false;
    return (p);
}

t_pattern ring_pattern(t_tuple a, t_tuple b)
{
    t_pattern p;

    p.a = a;
    p.b = b;
    p.at = ring_at;
    p.transform = identity();
    p.inv_transform = identity();
    p.has_transform = false;
    return (p);
}

t_pattern checkers_pattern(t_tuple a, t_tuple b)
{
    t_pattern p;

    p.a = a;
    p.b = b;
    p.at = checkers_at;
    p.transform = identity();
    p.inv_transform = identity();
    p.has_transform = false;
    return (p);
}

t_tuple stripe_at(t_pattern pattern, t_tuple point)
{
    if (((int)floor(point.x)) % 2 == 0)
        return (pattern.a);
    return (pattern.b);
}

t_tuple ring_at(t_pattern pattern, t_tuple point)
{
    float r;

    r = sqrtf(point.x * point.x + point.z * point.z);
    if (((int)floorf(r)) % 2 == 0)
        return (pattern.a);
    return (pattern.b);
}

t_tuple checkers_at(t_pattern pattern, t_tuple point)
{
    int sum;
    int xi, yi, zi;
    float epsilon = 0.00001f;

    // Add small epsilon to avoid floating point boundary issues
    xi = (int)floor(point.x + epsilon);
    yi = (int)floor(point.y + epsilon);
    zi = (int)floor(point.z + epsilon);
    
    sum = xi + yi + zi;
    if (sum % 2 == 0)
        return (pattern.a);
    return (pattern.b);
}
