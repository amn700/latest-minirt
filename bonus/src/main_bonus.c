/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/21 07:05:35 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT_bonus.h"

 void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_T)
		mlx_image_to_ppm(data->img, "output.ppm", 255);
	if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_ESCAPE)
	{
		mlx_close_window(data->ptr);
		exit(0);
	}
}

 int	initialize_mlx(t_data *data, t_camera cam)
{
	data->ptr = mlx_init((int)cam.h_view, (int)cam.v_view, "miniRT", false);
	if (!data->ptr)
	{
		printf("❌ Failed to initialize MLX\n");
		return (0);
	}
	data->img = render(data->ptr, cam, data->world);
	if (!data->img)
	{
		printf("❌ Failed to create image\n");
		mlx_terminate(data->ptr);
		return (0);
	}
	if (mlx_image_to_window(data->ptr, data->img, 0, 0) < 0)
	{
		printf("❌ Failed to display image\n");
		mlx_terminate(data->ptr);
		return (0);
	}
	return (1);
}

 int	setup_world(t_data *data, t_camera *cam)
{
	data->world = new_world();
	data->world.objects = data->object;
	data->world.lights = data->light;
	data->world.ambient_color = data->ambl.color;
	*cam = camera(1280, 720, data->cam.fov * M_PI / 180.0);
	cam->transform = view_transformation(data->cam.from, data->cam.to,
			data->cam.up);
	return (1);
}

void	free_objects_and_lights(t_data *data)
{
	t_object	*obj;
	t_object	*next_obj;
	t_light		*light;
	t_light		*next_light;

	obj = data->object;
	while (obj)
	{
		next_obj = obj->next;
		free(obj);
		obj = next_obj;
	}
	light = data->light;
	while (light)
	{
		next_light = light->next;
		free(light);
		light = next_light;
	}
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_camera	cam;

	data = (t_data){};
	if (argc != 2)
	{
		printf("❌ Usage: %s <scene_file.rt>\n", argv[0]);
		return (1);
	}
	if (!complete_parsing(argv[1], &data))
		return (1);
	printf("\n✅ Parsing completed successfully!\n");
	setup_world(&data, &cam);
	printf("\nRendering scene from file...\n");
	if (!initialize_mlx(&data, cam))
		return (free_objects_and_lights(&data), 1);
	free_objects_and_lights(&data);
	mlx_key_hook(data.ptr, key_hook, &data);
	printf("✅ Scene rendered. Press 'T' to save to output.ppm\n");
	mlx_loop(data.ptr);
	mlx_delete_image(data.ptr, data.img);
	mlx_terminate(data.ptr);
	return (0);
}
