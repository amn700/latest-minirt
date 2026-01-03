/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT_bonus.h"

static void	handle_screenshot(t_data *data)
{
	if (data->img && data->state == STATE_DISPLAY)
	{
		mlx_image_to_ppm(data->img, "screenshot.ppm", 255);
		printf("📸 Screenshot saved to screenshot.ppm\n");
	}
	else if (data->img && data->render_mode == MODE_PREVIEW
		&& data->render_complete)
	{
		mlx_image_to_ppm(data->img, "screenshot.ppm", 255);
		printf("📸 Screenshot saved to screenshot.ppm\n");
	}
}

static void	handle_space(t_data *data)
{
	if (data->render_mode == MODE_LOAD && data->state == STATE_WAITING)
	{
		show_rendered_image(data);
		printf("✅ Showing rendered scene\n");
	}
}

static void	handle_escape(t_data *data)
{
	if (data->state == STATE_RENDERING)
		stop_threaded_render(data);
	mlx_close_window(data->ptr);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE)
		handle_escape(data);
	else if (keydata.key == MLX_KEY_T)
		handle_screenshot(data);
	else if (keydata.key == MLX_KEY_SPACE)
		handle_space(data);
}

void	loop_hook(void *param)
{
	t_data	*data;
	int		i;

	data = (t_data *)param;
	if (data->state != STATE_RENDERING)
		return ;
	if (data->render_mode == MODE_LOAD)
		update_progress_bar(data);
	if (data->lines_done >= data->total_lines && !data->render_complete)
	{
		if (data->threads_started)
		{
			i = 0;
			while (i < NUM_THREADS)
			{
				pthread_join(data->threads[i], NULL);
				i++;
			}
			data->threads_started = false;
		}
		if (data->mutex_initialized)
		{
			pthread_mutex_destroy(&data->render_mutex);
			data->mutex_initialized = false;
		}
		data->render_complete = true;
		if (data->render_mode == MODE_PREVIEW)
		{
			data->state = STATE_DISPLAY;
			printf("✅ Rendering complete! Press T for screenshot\n");
		}
		else
		{
			data->state = STATE_WAITING;
			printf("✅ Rendering complete! Press SPACE to view\n");
		}
	}
}

int	setup_world(t_data *data, t_camera *cam)
{
	data->world = new_world();
	data->world.objects = data->object;
	data->world.lights = data->light;
	data->world.ambient_color = data->ambl.color;
	*cam = camera(1920, 1080, data->cam.fov * M_PI / 180.0);
	cam->transform = view_transformation(data->cam.from, data->cam.to,
			data->cam.up);
	data->cam = *cam;
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
		if (obj->type == OBJ_SPHERE)
			free_material_textures(&obj->shape.sp.material);
		else if (obj->type == OBJ_PLANE)
			free_material_textures(&obj->shape.pl.material);
		else if (obj->type == OBJ_CYLINDER)
			free_material_textures(&obj->shape.cy.material);
		else if (obj->type == OBJ_CONE)
			free_material_textures(&obj->shape.co.material);
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

static int	get_mode_selection(void)
{
	char	input[16];
	int		mode;

	printf("\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║           miniRT Bonus v2.0            ║\n");
	printf("╠════════════════════════════════════════╣\n");
	printf("║  [1] PREVIEW MODE                      ║\n");
	printf("║      Real-time threaded rendering      ║\n");
	printf("║      Watch the scene build live        ║\n");
	printf("║                                        ║\n");
	printf("║  [2] LOAD MODE                         ║\n");
	printf("║      Background rendering              ║\n");
	printf("║      Progress bar + SPACE to reveal    ║\n");
	printf("╠════════════════════════════════════════╣\n");
	printf("║  Controls: ESC=Exit  T=Screenshot      ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("Enter mode (1 or 2): ");
	fflush(stdout);
	if (!fgets(input, sizeof(input), stdin))
		return (MODE_PREVIEW);
	mode = atoi(input);
	if (mode != MODE_PREVIEW && mode != MODE_LOAD)
		mode = MODE_PREVIEW;
	return (mode);
}

static int	init_preview_mode(t_data *data)
{
	data->ptr = mlx_init((int)data->cam.h_view, (int)data->cam.v_view,
			"miniRT - Preview Mode", false);
	if (!data->ptr)
		return (0);
	data->img = mlx_new_image(data->ptr, (int)data->cam.h_view,
			(int)data->cam.v_view);
	if (!data->img)
		return (mlx_terminate(data->ptr), 0);
	mlx_image_to_window(data->ptr, data->img, 0, 0);
	data->state = STATE_RENDERING;
	start_threaded_render(data);
	printf("🎬 Preview mode: Rendering with %d threads...\n", NUM_THREADS);
	return (1);
}

static int	init_load_mode(t_data *data)
{
	data->ptr = mlx_init((int)data->cam.h_view, (int)data->cam.v_view,
			"miniRT - Load Mode", false);
	if (!data->ptr)
		return (0);
	data->img = mlx_new_image(data->ptr, (int)data->cam.h_view,
			(int)data->cam.v_view);
	if (!data->img)
		return (mlx_terminate(data->ptr), 0);
	data->img->enabled = false;
	mlx_image_to_window(data->ptr, data->img, 0, 0);
	draw_front_page(data);
	draw_progress_bar(data);
	data->state = STATE_RENDERING;
	start_threaded_render(data);
	printf("📊 Load mode: Rendering with %d threads...\n", NUM_THREADS);
	return (1);
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
	printf("✅ Parsing completed successfully!\n");
	data.render_mode = get_mode_selection();
	setup_world(&data, &cam);
	if (data.render_mode == MODE_PREVIEW)
	{
		if (!init_preview_mode(&data))
			return (free_objects_and_lights(&data), 1);
	}
	else
	{
		if (!init_load_mode(&data))
			return (free_objects_and_lights(&data), 1);
	}
	mlx_key_hook(data.ptr, key_hook, &data);
	mlx_loop_hook(data.ptr, loop_hook, &data);
	mlx_loop(data.ptr);
	if (data.state == STATE_RENDERING)
		stop_threaded_render(&data);
	free_objects_and_lights(&data);
	if (data.img)
		mlx_delete_image(data.ptr, data.img);
	mlx_terminate(data.ptr);
	return (0);
}
