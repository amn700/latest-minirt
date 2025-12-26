/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchaib <mohchaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by amn700            #+#    #+#             */
/*   Updated: 2025/09/17 11:52:21 by mohchaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <unistd.h>
# include <float.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
// # include "parsing.h"
// # include "execution.h"

#define HEIGHT 550
#define WIDTH  900

// int	main(int argc, char **argv)
// {
// 	t_data data;
// 	printf("miniRT - 42 Project\n");
// 	if (argc < 2)
// 	{
// 		printf("Usage: %s <scene_file.rt>\n", argv[0]);
// 		return (1);
// 	}
// 	// if (!complete_parsing(argv[1], &data))
// 	// 	return 1;
// 	data.ptr = mlx_init(WIDTH, HEIGHT, "miniRT", false);
// 	data.img = mlx_new_image(data.ptr, WIDTH, HEIGHT);
	
// 	t_tuple color = {1.0, 1.0, 0.0, 1.0};
// 	int i = 0; 
//     while (i < WIDTH)
//     {
// 		int j = 0; 
// 		while (j < HEIGHT)
// 		{
// 			mlx_put_pixel(data.img,  i, j , convert_color(color));
// 			j++;
// 		}
// 		i++;
//     }
	
// 	mlx_image_to_window(data.ptr, data.img, 0 , 0);
// 	// draw_line((t_screen){0,0}, (t_screen){800, 400}, data.img);
// 	// draw_line((t_screen){0,400}, (t_screen){800, 0}, data.img);
// 	// draw_line((t_screen){0,200}, (t_screen){800, 200}, data.img);
// 	// draw_line((t_screen){400,0}, (t_screen){400, 400}, data.img);
// 	printf("Scene file: %s\n", argv[1]);
// 	mlx_loop(data.ptr);
// 	return (0);
// }


bool	mlx_image_to_ppm(mlx_image_t *img, char *string, unsigned int range)
{
	FILE *output = fopen (string ,"w");
	if (!output)
		return false;
	fprintf(output, "P3\n");
	fprintf(output, "%i %i\n", img->width, img->height);
	fprintf(output, "%u\n", range);
	int line_len = 0;
	for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4)
	{
		uint8_t r = img->pixels[i + 0];
		uint8_t g = img->pixels[i + 1];
		uint8_t b = img->pixels[i + 2];

		line_len += fprintf(output, "%3u %3u %3u ", r, g, b);
		if ((i / 4 + 1) % img->width == 0)
			fprintf(output, "\n");
	}
		fprintf(output, "\n");
	fclose (output);
	return true;
}

typedef struct
{
    t_tuple point;
    t_tuple velocity;
}           projectile;

typedef struct
{
    t_tuple gravity;
    t_tuple wind;
}           environemt;

projectile  tick(environemt env, projectile proj)
{
    t_tuple new_pos;
    t_tuple new_vec;

    new_pos = add_tuple(proj.point, proj.velocity);
    new_vec = add_tuple(add_tuple(env.wind, env.gravity), proj.velocity);
    return (projectile){new_pos, new_vec};
}


int main ()
{
	t_data data;
	data.ptr = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	data.img = mlx_new_image(data.ptr, WIDTH, HEIGHT);
	t_tuple velocity = (t_tuple){1, 1.8, 0, 0};
    projectile original_proj = {(t_tuple){0, 1, 0, 1}, tuple_scalar_mult(normalizing_vector(velocity), 11.25)};
    environemt original_env = {(t_tuple){0, -0.1, 0, 0}, (t_tuple){-0.01, 0, 0, 0}};
	projectile new_proj = original_proj;
	t_tuple color = {1.0, 0.0, 0.0, 1.0};
	int c = convert_color(color);
	while (new_proj.point.y > 0 && new_proj.point.x >= 0 && new_proj.point.x < WIDTH)
	{
		projectile dol_prj = new_proj;
		int screen_x_o = (int)dol_prj.point.x;
		int screen_y_o = HEIGHT - 1 - (int)dol_prj.point.y;
		
		new_proj = tick(original_env, new_proj);
		int screen_x = (int)new_proj.point.x;
		int screen_y = HEIGHT - 1 - (int)new_proj.point.y;

		if ((screen_x_o >= 0 && screen_x_o < WIDTH && screen_y_o >= 0 && screen_y_o < HEIGHT) &&
			(screen_x  >= 0 && screen_x  < WIDTH && screen_y  >= 0 && screen_y  < HEIGHT))
					draw_line((t_screen){screen_x_o , screen_y_o},(t_screen){screen_x,screen_y}, data.img, c);
		printf("x:%f\ty:%f\tz:%f\n", new_proj.point.x, new_proj.point.y, new_proj.point.z);
	}

	// int i = 0;
    // while (i < WIDTH)
    // {
	// 	int j = 0; 
	// 	while (j < HEIGHT)
	// 	{
	// 		mlx_put_pixel(data.img,  i, j , convert_color(color));
	// 		j++;
	// 	}
	// 	i++;
    // }
	mlx_image_to_ppm(data.img, "test.PPM", 255);
	mlx_image_to_window(data.ptr, data.img, 0 , 0);
	mlx_loop(data.ptr);
}

