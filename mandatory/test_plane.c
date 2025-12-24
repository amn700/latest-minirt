#include "includes/miniRT.h"
#include <stdio.h>

int main() {
    t_plane pl = plane();
    t_ray ray = {(t_tuple){0, 1, -5, 1}, (t_tuple){0, -0.5, 1, 0}};
    ray.direction = normalizing_vector(ray.direction);
    
    t_inters *inters = NULL;
    intersect_plane(&pl, ray, &inters);
    
    if (inters) {
        printf("Intersection t: %f\n", inters->t);
        t_tuple point = position(ray, inters->t);
        printf("Intersection point: (%f, %f, %f)\n", point.x, point.y, point.z);
        
        t_tuple normal = plane_normal_at(pl);
        printf("Normal: (%f, %f, %f)\n", normal.x, normal.y, normal.z);
    } else {
        printf("No intersection\n");
    }
    return 0;
}
