/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong_lighting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 11:36:49 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 11:53:10 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_light	point_light(t_tuple point, t_tuple color)
{
	return ((t_light){.color = color, .origin = point});
}

void	lighting_else(t_light_params params, t_light_vars *vars)
{
	vars->diffuse = tuple_scalar_mult(tuple_scalar_mult(vars->effective_color,
				params.material.diffuse), vars->ldn * params.light.brightness);
	vars->reflectv = reflect(negate_tuple(vars->lightv), params.normalv);
	vars->rde = vecs_dot_product(vars->reflectv, params.eyev);
	if (vars->rde <= 0)
		vars->specular = (t_tuple){0, 0, 0, 0};
	else
	{
		vars->factor = pow(vars->rde, params.material.shininess);
		vars->specular = tuple_scalar_mult(tuple_scalar_mult(tuple_scalar_mult(
						params.light.color, params.light.brightness),
					params.material.specular), vars->factor);
	}
}

t_tuple	lighting(t_light_params params)
{
	t_light_vars	vars;

	vars.ambient = hadamard_product(tuple_scalar_mult(params.material.color,
				params.material.ambient), params.ambient_color);
	if (params.in_shadow)
		return (vars.ambient.w = 0, vars.ambient);
	vars.effective_color = hadamard_product(params.material.color,
			params.light.color);
	vars.lightv = normalizing_vector(substract_tuple(params.light.origin,
				params.position));
	vars.ldn = vecs_dot_product(vars.lightv, params.normalv);
	if (vars.ldn < 0)
	{
		vars.diffuse = (t_tuple){0, 0, 0, 0};
		vars.specular = (t_tuple){0, 0, 0, 0};
	}
	else
		lighting_else(params, &vars);
	vars.result = add_tuple(add_tuple(vars.ambient, vars.diffuse),
			vars.specular);
	vars.result.w = 0;
	return (vars.result);
}
