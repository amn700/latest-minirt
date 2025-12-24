#include "../../includes/miniRT_bonus.h"

t_light point_light(t_tuple point, t_tuple color)
{
    return (t_light){.color = color, .origin = point};
}

typedef struct s_lighting_components
{
	t_tuple	ambient;
	t_tuple	diffuse;
	t_tuple	specular;
}	t_lighting_components;

static t_lighting_components	compute_lighting_components(t_material material, \
	t_light light, t_tuple position, t_tuple eyev, t_tuple normalv, \
	bool in_shadow, t_tuple ambient_color)
{
	t_lighting_components	result;
	t_tuple				effective_color;
	t_tuple					lightv;
	float					ldn;
	t_tuple					reflectv;
	float					rde;
	float					factor;
	t_tuple					color;

	color = material.color;
	if (material.pattern.at)
	{
		t_tuple pos = position;
		if (material.pattern.has_transform)
			pos = multiply_matrix_by_tuple(material.pattern.inv_transform, position);
		color = material.pattern.at(material.pattern, pos);
	}
	result.ambient = hadamard_product(tuple_scalar_mult(color, \
		material.ambient), ambient_color);
	result.diffuse = (t_tuple){0, 0, 0, 0};
	result.specular = (t_tuple){0, 0, 0, 0};
	if (in_shadow)
		return (result);
	effective_color = hadamard_product(color, light.color);
	lightv = normalizing_vector(substract_tuple(light.origin, position));
	ldn = vecs_dot_product(lightv, normalv);
	if (ldn >= 0)
		result.diffuse = tuple_scalar_mult(tuple_scalar_mult(effective_color, \
			material.diffuse), ldn * light.brightness);
	reflectv = reflect(negate_tuple(lightv), normalv);
	rde = vecs_dot_product(reflectv, eyev);
	if (rde > 0)
	{
		factor = pow(rde, material.shininess);
		result.specular = tuple_scalar_mult(tuple_scalar_mult(
			tuple_scalar_mult(light.color, light.brightness), \
			material.specular), factor);
	}
	return (result);
}

t_tuple lighting(t_material material, t_light light, t_tuple position, t_tuple eyev, t_tuple normalv, bool in_shadow, t_tuple ambient_color)
{
	t_lighting_components	comp;
	t_tuple					result;

	comp = compute_lighting_components(material, light, position, eyev, \
		normalv, in_shadow, ambient_color);
	result = add_tuple(add_tuple(comp.ambient, comp.diffuse), comp.specular);
	result.w = 0;
	return (result);
}

t_tuple reflected_color(t_world world, t_comps *comps, int depth)
{
    t_ray reflect_ray;
    t_tuple color;
    float reflective;

    if (depth <= 0)
        return ((t_tuple){0,0,0,0});
    
    // Get reflective value based on object type
    if (comps->obj->type == OBJ_SPHERE)
        reflective = comps->obj->shape.sp.material.reflective;
    else if (comps->obj->type == OBJ_PLANE)
        reflective = comps->obj->shape.pl.material.reflective;
    else if (comps->obj->type == OBJ_CYLINDER)
        reflective = comps->obj->shape.cy.material.reflective;
    else if (comps->obj->type == OBJ_CONE)
        reflective = comps->obj->shape.co.material.reflective;
    else
        reflective = 0.0;
    
    if (reflective <= 0.0)
        return ((t_tuple){0,0,0,0});
    
    reflect_ray = (t_ray){comps->over_point, comps->reflectv};
    color = color_at(world, reflect_ray, depth - 1);
    
    return tuple_scalar_mult(color, reflective);
}

static float	get_transparency(t_comps *comps)
{
	if (comps->obj->type == OBJ_SPHERE)
		return (comps->obj->shape.sp.material.transparency);
	else if (comps->obj->type == OBJ_PLANE)
		return (comps->obj->shape.pl.material.transparency);
	else if (comps->obj->type == OBJ_CYLINDER)
		return (comps->obj->shape.cy.material.transparency);
	else if (comps->obj->type == OBJ_CONE)
		return (comps->obj->shape.co.material.transparency);
	return (0.0);
}

static bool	is_total_internal_reflection(t_comps *comps, float n_ratio, float *cos_i, float *sin2_t)
{
	*cos_i = vecs_dot_product(comps->eyev, comps->normalv);
	*sin2_t = n_ratio * n_ratio * (1 - (*cos_i) * (*cos_i));
	if (*sin2_t > 1.0)
		return (true);
	return (false);
}

t_tuple	refracted_color(t_world world, t_comps *comps, int depth)
{
	float	transparency;
	float	n_ratio;
	float	cos_i;
	float	sin2_t;
	float	cos_t;
	t_tuple	direction;
	t_ray	refract_ray;

	if (depth <= 0)
		return ((t_tuple){0, 0, 0, 0});
	transparency = get_transparency(comps);
	if (transparency <= 0.0)
		return ((t_tuple){0, 0, 0, 0});
	n_ratio = comps->n1 / comps->n2;
	if (is_total_internal_reflection(comps, n_ratio, &cos_i, &sin2_t))
		return ((t_tuple){0, 0, 0, 0});
	cos_t = sqrtf(1.0 - sin2_t);
	direction = substract_tuple(
		tuple_scalar_mult(comps->normalv, (n_ratio * cos_i - cos_t)),
		tuple_scalar_mult(comps->eyev, n_ratio));
	refract_ray = (t_ray){comps->under_point, direction};
	return (tuple_scalar_mult(color_at(world, refract_ray, depth - 1), transparency));
}
