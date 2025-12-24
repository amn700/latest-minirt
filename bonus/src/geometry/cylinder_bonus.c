#include "../../includes/miniRT_bonus.h"

void	ft_add_intersection(t_inters **lst, t_inters *new);
t_inters	*new_intersection(float t, t_object *obj);

// typedef struct s_cylinder
// {
// 	t_tuple	center;
// 	t_tuple	axis;
// 	float	diameter;
// 	float	height;
// 	t_matrix trans;
// 	t_material material;
// }			t_cylinder;

t_cylinder cylinder(void)
{
    return (t_cylinder){
        .center = (t_tuple){0,0,0,1},
        .axis = (t_tuple){0,1,0,0},
        .diameter = 2,
        .height = INFINITY,
        .trans = identity(),
        .material = material(),
        .minimum = -INFINITY,
        .maximum = INFINITY,
    };
}

bool    intersect_cylinder(t_cylinder *cyl, t_ray ray, t_inters **intersections)
{
    // Calculate intersection with infinite cylinder along Y axis
    float a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    
    // Ray is parallel to the cylinder's axis
    if (fabs(a) < 1e-6)
        return true; // No side intersections, but may have cap intersections
    
    float b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    float c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0)
        return true; // No intersections
    
    float sqrt_disc = sqrt(discriminant);
    float t0 = (-b - sqrt_disc) / (2 * a);
    float t1 = (-b + sqrt_disc) / (2 * a);
    
    if (t0 > t1)
    {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // Check if intersections are within cylinder height bounds
    float y0 = ray.origin.y + t0 * ray.direction.y;
    if (y0 > cyl->minimum && y0 < cyl->maximum)
    {
        // Create separate object for first intersection
        t_object *obj0 = malloc(sizeof(t_object));
        if (obj0)
        {
            obj0->type = OBJ_CYLINDER;
            obj0->shape.cy = *cyl;
            obj0->next = NULL;
            
            t_inters *inters0 = new_intersection(t0, obj0);
            if (inters0)
                ft_add_intersection(intersections, inters0);
            else
                free(obj0);
        }
    }
    
    float y1 = ray.origin.y + t1 * ray.direction.y;
    if (y1 > cyl->minimum && y1 < cyl->maximum)
    {
        // Create separate object for second intersection
        t_object *obj1 = malloc(sizeof(t_object));
        if (obj1)
        {
            obj1->type = OBJ_CYLINDER;
            obj1->shape.cy = *cyl;
            obj1->next = NULL;
            
            t_inters *inters1 = new_intersection(t1, obj1);
            if (inters1)
                ft_add_intersection(intersections, inters1);
            else
                free(obj1);
        }
    }
    
    return true;
}


t_tuple cylinder_normal_at(t_cylinder cyl, t_tuple point)
{
    // Transform point to object space
    t_matrix inverse = inverse_matrix(cyl.trans);
    t_tuple object_point = multiply_matrix_by_tuple(inverse, point);
    
    // For a cylinder along Y axis, normal is perpendicular to Y
    // At the caps (if implemented), normal would be (0, ±1, 0)
    // On the sides, normal is (x, 0, z) in object space
    t_tuple object_normal = (t_tuple){object_point.x, 0, object_point.z, 0};
    
    // Transform normal back to world space using inverse transpose
    t_tuple world_normal = multiply_matrix_by_tuple(transposing_matrix(inverse), object_normal);
    world_normal.w = 0;  // Ensure it's a vector
    
    return normalizing_vector(world_normal);
}

