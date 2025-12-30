#include "../../includes/miniRT.h"

t_light point_light(t_tuple point, t_tuple color)
{
    return (t_light){.color = color, .origin = point};
}

t_tuple lighting(t_material material, t_light light, t_tuple position, t_tuple eyev, t_tuple normalv, bool in_shadow, t_tuple ambient_color)
{
    // Ambient is independent of light source - only uses material color and ambient color
    t_tuple ambient = hadamard_product(tuple_scalar_mult(material.color, material.ambient), ambient_color);
    
    // If in shadow, return only ambient lighting
    if (in_shadow)
    {
        ambient.w = 0;
        return ambient;
    }
    
    // For diffuse and specular, we use effective_color (material * light color)
    t_tuple effective_color = hadamard_product(material.color , light.color);
    t_tuple lightv = normalizing_vector(substract_tuple(light.origin, position));
    
    float ldn = vecs_dot_product(lightv, normalv);
    t_tuple diffuse;
    t_tuple specular;
    if (ldn < 0)
    {
        diffuse = (t_tuple){0, 0 ,0, 0};
        specular = (t_tuple){0, 0 ,0, 0};
    }
    else
    {
        diffuse = tuple_scalar_mult(tuple_scalar_mult(effective_color, material.diffuse), ldn * light.brightness);
        
        t_tuple reflectv = reflect(negate_tuple(lightv), normalv);
        float rde = vecs_dot_product(reflectv, eyev);
        
        if (rde <= 0)
            specular = (t_tuple){0, 0 ,0, 0};
        else
        {
            float factor = pow(rde, material.shininess);
            specular = tuple_scalar_mult(tuple_scalar_mult(tuple_scalar_mult(light.color, light.brightness), material.specular), factor);
        }
    }
    t_tuple result = add_tuple(add_tuple(ambient, diffuse), specular);
    result.w = 0;  // Ensure w component is 0 for colors
    return result;
}