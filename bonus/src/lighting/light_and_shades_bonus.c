#include "../../includes/miniRT_bonus.h"

// bool        intersect(t_sphere sp, t_ray ray, t_inters **intersections);
// t_ray       transform_ray(t_ray ray, t_matrix m);
// t_matrix	translation(float x, float y, float z);
// t_matrix	shearing(t_propo propotion);
// t_matrix	identity(void);
// float	hit(t_inters *list);
// void	sort_intersections(t_inters **lst);
// t_sphere	sp_transform(t_matrix m, t_sphere sp);

// typedef struct t_shade_info
// {
//     t_object	*object;
//     t_tuple		point;
//     t_tuple		eyev;
//     t_tuple		normalv;
//     t_tuple     lightv;
//     float       t;
//     bool        inside;
//     t_tuple     over_point;
//     t_tuple     under_point;
//     t_tuple     reflectv;
//     float       n1;
//     float       n2;
//     bool        in_shadow;
//     t_tuple     color;
//     float       ambient;
//     float       diffuse;
//     float       specular;
//     float       shininess;
//     t_tuple     light_position;
//     t_tuple     light_intensity;
// }	t_shade_info;

// t_tuple point(int pixel_x, int pixel_y, int canvas_width)
// {
//     // Book's Hint #2 parameters:
//     float wall_size = 7.0f;                           // 7×7 unit wall
//     float pixel_size = wall_size / canvas_width;      // 7.0 / 400 = 0.0175 units per pixel
//     float half = wall_size / 2.0f;                    // 3.5 (center to edge)
    
//     // Convert pixel coordinates to world coordinates:
//     // Left = -half, Right = +half
//     float world_x = -half + pixel_size * pixel_x;
    
//     // Top = +half, Bottom = -half (Y flipped as per book)
//     float world_y = half - pixel_size * pixel_y;
    
//     // Wall at z = 10 (as per book's hint)
//     return (t_tuple){world_x, world_y, 10, 1};
// }

// t_tuple normal_at(t_sphere sp, t_tuple p)
// {
//     return normalizing_vector(substract_tuple(p, sp.center));
// }

// int main ()
// {
//     t_sphere sp = sphere();
//     sp = sp_transform(translation(0, 1 ,0), sp);
    
//     // Create a point in 3D space (not using the pixel conversion function)
//     t_tuple test_point = (t_tuple){0, 1.70711, -0.70711, 1};
//     t_tuple N = normal_at(sp, test_point);
    
//     printf("Normal: %f %f %f %f\n", N.x, N.y, N.z, N.w);
// }
