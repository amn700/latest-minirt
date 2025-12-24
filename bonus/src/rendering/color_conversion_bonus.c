#include "../../includes/miniRT_bonus.h"

int convert_color(t_tuple c)
{
    // Clamp RGB values to 0.0-1.0 range to prevent overflow
    float r_clamped = c.x < 0.0f ? 0.0f : (c.x > 1.0f ? 1.0f : c.x);
    float g_clamped = c.y < 0.0f ? 0.0f : (c.y > 1.0f ? 1.0f : c.y);
    float b_clamped = c.z < 0.0f ? 0.0f : (c.z > 1.0f ? 1.0f : c.z);
    
    uint8_t r = (uint8_t)(r_clamped * 255);
    uint8_t g = (uint8_t)(g_clamped * 255);
    uint8_t b = (uint8_t)(b_clamped * 255);
    uint8_t a = 255;

    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | a;
}