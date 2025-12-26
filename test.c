# include "MLX42/include/MLX42/MLX42.h"
# include <stdio.h>

typedef struct mlx_image
{
	const uint32_t	width;
	const uint32_t	height;
	uint8_t*		pixels;
	mlx_instance_t*	instances;
	size_t			count;
	bool			enabled;
	void*			context;
}	mlx_image_t;

int main()
{
    mlx_t *ptr = mlx_init(1920,1080,"test", 0);
    mlx_texture_t *jpeg = mlx_load_png("z_textures/red_mini.png");
    mlx_image_t *img = mlx_texture_to_image(ptr, jpeg);
    mlx_image_to_window(ptr, img, 0,0);

    printf("width: %u\n", img->width);
    printf("height: %u\n", img->height);
    int i = 0;
    int size = img->width * img->height ;
    // printf("size:%i\n", size);
    // printf("count:%zu\n", img->count);
    while (i < size)
    {
        printf("r:{%u}",  img->pixels[i+0]);
        printf("g:{%u}",  img->pixels[i+1]);
        printf("b:{%u}",  img->pixels[i+2]);
        printf("a:{%u}\n",img->pixels[i+3]);
        i += 4;
    }
    mlx_loop(ptr);
}
