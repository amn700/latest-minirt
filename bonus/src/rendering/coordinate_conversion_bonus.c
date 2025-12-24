#include "../../includes/miniRT_bonus.h"

t_tuple point(int pixel_x, int pixel_y, int canvas_width)
{
    // Book's Hint #2 parameters:
    float wall_size = 7.0f;                           // 7×7 unit wall
    float pixel_size = wall_size / canvas_width;      // 7.0 / 400 = 0.0175 units per pixel
    float half = wall_size / 2.0f;                    // 3.5 (center to edge)
    
    // Convert pixel coordinates to world coordinates:
    // Left = -half, Right = +half
    float world_x = -half + pixel_size * pixel_x;
    
    // Top = +half, Bottom = -half (Y flipped as per book)
    float world_y = half - pixel_size * pixel_y;
    
    // Wall at z = 10 (as per book's hint)
    return (t_tuple){world_x, world_y, 10, 1};
}