/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amn <amn@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by amn               #+#    #+#             */
/*   Updated: 2025/12/02 22:48:42 by amn              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT_bonus.h"

static void	print_tuple(const char *label, t_tuple t)
{
	printf("%s: (%.2f, %.2f, %.2f, %.2f)\n", label, t.x, t.y, t.z, t.w);
}

static void	print_color(const char *label, t_tuple color)
{
	printf("%s: RGB(%.0f, %.0f, %.0f)\n", label, 
		color.x * 255, color.y * 255, color.z * 255);
}

static void	check_ambient_light(t_ambient_light ambl)
{
	printf("\n=== AMBIENT LIGHT ===\n");
	printf("Lighting ratio: %.2f", ambl.l_ratio);
	if (ambl.l_ratio < 0.0 || ambl.l_ratio > 1.0)
		printf(" ⚠️ WARNING: Should be in range [0.0, 1.0]");
	printf("\n");
	print_color("Color", ambl.color);
	if (ambl.color.x < 0 || ambl.color.x > 1 || 
		ambl.color.y < 0 || ambl.color.y > 1 || 
		ambl.color.z < 0 || ambl.color.z > 1)
		printf("⚠️ WARNING: Color values should be normalized [0.0, 1.0]\n");
}

static void	check_camera(t_camera cam)
{
	printf("\n=== CAMERA ===\n");
	print_tuple("Position (from)", cam.from);
	print_tuple("View point (to)", cam.to);
	print_tuple("Up vector", cam.up);
	printf("FOV: %.2f degrees", cam.fov);
	if (cam.fov < 0 || cam.fov > 180)
		printf(" ⚠️ WARNING: Should be in range [0, 180]");
	printf("\n");
	printf("Pixel size: %.6f\n", cam.pixel_size);
	printf("Half width: %.2f, Half height: %.2f\n", cam.half_width, cam.half_height);
	printf("Horizontal view: %.2f, Vertical view: %.2f\n", cam.h_view, cam.v_view);
}

static void	check_lights(t_light *light)
{
	int	count;

	count = 0;
	printf("\n=== LIGHTS ===\n");
	while (light)
	{
		printf("\n--- Light #%d ---\n", count + 1);
		print_tuple("Position", light->origin);
		printf("Brightness ratio: %.2f", light->brightness);
		if (light->brightness < 0.0 || light->brightness > 1.0)
			printf(" ⚠️ WARNING: Should be in range [0.0, 1.0]");
		printf("\n");
		print_color("Color", light->color);
		if (light->color.x < 0 || light->color.x > 1 || 
			light->color.y < 0 || light->color.y > 1 || 
			light->color.z < 0 || light->color.z > 1)
			printf("⚠️ WARNING: Color values should be normalized [0.0, 1.0]\n");
		light = light->next;
		count++;
	}
	printf("\nTotal lights: %d\n", count);
}

static void	check_sphere(t_sphere sp, int index)
{
	printf("\n--- Sphere #%d ---\n", index);
	print_tuple("Center", sp.center);
	printf("Diameter: %.2f", sp.diam);
	if (sp.diam <= 0)
		printf(" ⚠️ WARNING: Should be positive");
	printf("\n");
	print_color("Material color", sp.material.color);
	printf("Material properties:\n");
	printf("  Ambient: %.2f\n", sp.material.ambient);
	printf("  Diffuse: %.2f\n", sp.material.diffuse);
	printf("  Specular: %.2f\n", sp.material.specular);
	printf("  Shininess: %.2f\n", sp.material.shininess);
}

static void	check_plane(t_plane pl, int index)
{
	float	magnitude;

	printf("\n--- Plane #%d ---\n", index);
	print_tuple("Point", pl.point);
	print_tuple("Normal vector", pl.normal);
	magnitude = vector_magnitude(pl.normal);
	printf("Normal magnitude: %.2f", magnitude);
	if (magnitude < 0.99 || magnitude > 1.01)
		printf(" ⚠️ WARNING: Should be normalized (magnitude = 1.0)");
	printf("\n");
	if (pl.normal.x < -1 || pl.normal.x > 1 || 
		pl.normal.y < -1 || pl.normal.y > 1 || 
		pl.normal.z < -1 || pl.normal.z > 1)
		printf("⚠️ WARNING: Normal components should be in range [-1, 1]\n");
	print_color("Material color", pl.material.color);
	printf("Material properties:\n");
	printf("  Ambient: %.2f\n", pl.material.ambient);
	printf("  Diffuse: %.2f\n", pl.material.diffuse);
	printf("  Specular: %.2f\n", pl.material.specular);
	printf("  Shininess: %.2f\n", pl.material.shininess);
}

static void	check_cylinder(t_cylinder cy, int index)
{
	float	magnitude;

	printf("\n--- Cylinder #%d ---\n", index);
	print_tuple("Center", cy.center);
	print_tuple("Axis vector", cy.axis);
	magnitude = vector_magnitude(cy.axis);
	printf("Axis magnitude: %.2f", magnitude);
	if (magnitude < 0.99 || magnitude > 1.01)
		printf(" ⚠️ WARNING: Should be normalized (magnitude = 1.0)");
	printf("\n");
	if (cy.axis.x < -1 || cy.axis.x > 1 || 
		cy.axis.y < -1 || cy.axis.y > 1 || 
		cy.axis.z < -1 || cy.axis.z > 1)
		printf("⚠️ WARNING: Axis components should be in range [-1, 1]\n");
	printf("Diameter: %.2f", cy.diameter);
	if (cy.diameter <= 0)
		printf(" ⚠️ WARNING: Should be positive");
	printf("\n");
	printf("Height: %.2f", cy.height);
	if (cy.height <= 0)
		printf(" ⚠️ WARNING: Should be positive");
	printf("\n");
	printf("Minimum: %.2f, Maximum: %.2f\n", cy.minimum, cy.maximum);
	print_color("Material color", cy.material.color);
	printf("Material properties:\n");
	printf("  Ambient: %.2f\n", cy.material.ambient);
	printf("  Diffuse: %.2f\n", cy.material.diffuse);
	printf("  Specular: %.2f\n", cy.material.specular);
	printf("  Shininess: %.2f\n", cy.material.shininess);
}

static void	check_objects(t_object *obj)
{
	int	sp_count;
	int	pl_count;
	int	cy_count;
	t_object *current;

	sp_count = 0;
	pl_count = 0;
	cy_count = 0;
	printf("\n=== OBJECTS ===\n");
	current = obj;
	while (current)
	{
		if (current->type == OBJ_SPHERE)
		{
			check_sphere(current->shape.sp, ++sp_count);
		}
		else if (current->type == OBJ_PLANE)
		{
			check_plane(current->shape.pl, ++pl_count);
		}
		else if (current->type == OBJ_CYLINDER)
		{
			check_cylinder(current->shape.cy, ++cy_count);
		}
		current = current->next;
	}
	printf("\n--- Object Summary ---\n");
	printf("Total spheres: %d\n", sp_count);
	printf("Total planes: %d\n", pl_count);
	printf("Total cylinders: %d\n", cy_count);
	printf("Total objects: %d\n", sp_count + pl_count + cy_count);
}

void	check_parsed_data(t_data *data)
{
	printf("\n");
	printf("╔════════════════════════════════════════════════════╗\n");
	printf("║         PARSING VALIDATION REPORT                  ║\n");
	printf("╚════════════════════════════════════════════════════╝\n");

	check_ambient_light(data->ambl);
	check_camera(data->cam);
	check_lights(data->light);
	check_objects(data->object);

	printf("\n");
	printf("╔════════════════════════════════════════════════════╗\n");
	printf("║         END OF VALIDATION REPORT                   ║\n");
	printf("╚════════════════════════════════════════════════════╝\n");
	printf("\n");
}

void	print_matrix(const char *label, t_matrix m)
{
	printf("%s:\n", label);
	for (int i = 0; i < 4; i++)
	{
		printf("  [");
		for (int j = 0; j < 4; j++)
		{
			printf(" %7.3f", m.mtrx[i][j]);
		}
		printf(" ]\n");
	}
}

void	debug_world_state(t_world world, t_camera cam)
{
	printf("\n");
	printf("╔════════════════════════════════════════════════════╗\n");
	printf("║         WORLD STATE & CALCULATIONS DEBUG           ║\n");
	printf("╚════════════════════════════════════════════════════╝\n");
	
	printf("\n=== WORLD STATE ===\n");
	printf("Ambient color: RGB(%.3f, %.3f, %.3f)\n", 
		world.ambient_color.x, world.ambient_color.y, world.ambient_color.z);
	
	printf("\n=== CAMERA STATE ===\n");
	printf("Resolution: %.0fx%.0f\n", cam.h_view, cam.v_view);
	printf("FOV: %.2f radians (%.2f degrees)\n", cam.fov, cam.fov * 180.0 / M_PI);
	printf("Pixel size: %.6f\n", cam.pixel_size);
	printf("Half width: %.6f, Half height: %.6f\n", cam.half_width, cam.half_height);
	print_matrix("Camera Transform", cam.transform);
	
	printf("\n=== TRANSFORMATION MATRICES ===\n");
	
	t_object *obj = world.objects;
	int obj_num = 1;
	
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			printf("\n--- Sphere #%d ---\n", obj_num);
			printf("Center (parsed): (%.2f, %.2f, %.2f)\n", 
				obj->shape.sp.center.x, obj->shape.sp.center.y, obj->shape.sp.center.z);
			printf("Diameter: %.2f, Radius: %.2f\n", 
				obj->shape.sp.diam, obj->shape.sp.diam / 2.0);
			print_matrix("Transform", obj->shape.sp.trans);
			
			// Test transform application: origin should map to center
			t_tuple origin = {0, 0, 0, 1};
			t_tuple transformed = multiply_matrix_by_tuple(obj->shape.sp.trans, origin);
			printf("Origin (0,0,0) transformed to: (%.2f, %.2f, %.2f)\n",
				transformed.x, transformed.y, transformed.z);
		}
		else if (obj->type == OBJ_PLANE)
		{
			printf("\n--- Plane #%d ---\n", obj_num);
			printf("Point (parsed): (%.2f, %.2f, %.2f)\n",
				obj->shape.pl.point.x, obj->shape.pl.point.y, obj->shape.pl.point.z);
			printf("Normal (parsed): (%.2f, %.2f, %.2f)\n",
				obj->shape.pl.normal.x, obj->shape.pl.normal.y, obj->shape.pl.normal.z);
			print_matrix("Transform", obj->shape.pl.trans);
		}
		else if (obj->type == OBJ_CYLINDER)
		{
			printf("\n--- Cylinder #%d ---\n", obj_num);
			printf("Center (parsed): (%.2f, %.2f, %.2f)\n",
				obj->shape.cy.center.x, obj->shape.cy.center.y, obj->shape.cy.center.z);
			printf("Axis (parsed): (%.2f, %.2f, %.2f)\n",
				obj->shape.cy.axis.x, obj->shape.cy.axis.y, obj->shape.cy.axis.z);
			printf("Diameter: %.2f, Radius: %.2f, Height: %.2f\n",
				obj->shape.cy.diameter, obj->shape.cy.diameter / 2.0, obj->shape.cy.height);
			printf("Min: %.2f, Max: %.2f\n", obj->shape.cy.minimum, obj->shape.cy.maximum);
			print_matrix("Transform", obj->shape.cy.trans);
			
			// Test transform application
			t_tuple origin = {0, 0, 0, 1};
			t_tuple transformed = multiply_matrix_by_tuple(obj->shape.cy.trans, origin);
			printf("Origin (0,0,0) transformed to: (%.2f, %.2f, %.2f)\n",
				transformed.x, transformed.y, transformed.z);
		}
		
		obj = obj->next;
		obj_num++;
	}
	
	printf("\n");
	printf("╔════════════════════════════════════════════════════╗\n");
	printf("║         END OF DEBUG REPORT                        ║\n");
	printf("╚════════════════════════════════════════════════════╝\n");
	printf("\n");
}
