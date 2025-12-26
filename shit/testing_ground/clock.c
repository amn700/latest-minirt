# include "../MLX42/include/MLX42/MLX42.h"
# include "../mandatory/includes/miniRT.h"

int main ()
{
    mlx_t *ptr = mlx_init(800, 800, "clock", false);
    mlx_image_t *img = mlx_new_image(ptr, 800, 800);

    t_tuple p[12];
    for (int i = 0; i < 12; i++)
    {
        p[i].x = 0;
        p[i].y = 0;
        p[i].z = 0;
        p[i].w = 1;
        t_matrix trans = multiply_matrix(translation(800/2, 800/2, 0), multiply_matrix(rotation_z(to_radians(30 * i)),translation(0, 800/3, 0)));

        p[i] = multiply_matrix_by_tuple(trans, p[i]); 
    }

    
    for (int i = 0; i < 12; i++)
        mlx_put_pixel(img, p[i].x, p[i].y, -1);
    mlx_image_to_window(ptr, img, 0, 0);
    mlx_loop(ptr);
}
