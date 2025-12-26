#include "../../includes/miniRT.h"

# define M_PI		3.14159265358979323846

// Fluent API for chaining transformations
typedef struct s_transform_chain
{
	t_matrix	matrix;
}	t_transform_chain;

// Start the chain with identity matrix
t_transform_chain	identity(void)
{
	t_transform_chain chain;
	chain.matrix.size = 4;
	
	// Create identity matrix
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			chain.matrix.mtrx[row][col] = 0;
			if (col == row)
				chain.matrix.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	return chain;
}

// Chain rotation_x
t_transform_chain	chain_rotate_x(t_transform_chain chain, float theta)
{
	t_matrix rot = rotation_x(theta);
	chain.matrix = multiply_matrix(rot, chain.matrix);
	return chain;
}

// Chain rotation_y
t_transform_chain	chain_rotate_y(t_transform_chain chain, float theta)
{
	t_matrix rot = rotation_y(theta);
	chain.matrix = multiply_matrix(rot, chain.matrix);
	return chain;
}

// Chain rotation_z
t_transform_chain	chain_rotate_z(t_transform_chain chain, float theta)
{
	t_matrix rot = rotation_z(theta);
	chain.matrix = multiply_matrix(rot, chain.matrix);
	return chain;
}

// Chain scaling
t_transform_chain	chain_scale(t_transform_chain chain, float x, float y, float z)
{
	t_matrix scale = scaling(x, y, z);
	chain.matrix = multiply_matrix(scale, chain.matrix);
	return chain;
}

// Chain translation
t_transform_chain	chain_translate(t_transform_chain chain, float x, float y, float z)
{
	t_matrix trans = translation(x, y, z);
	chain.matrix = multiply_matrix(trans, chain.matrix);
	return chain;
}

// Chain shearing
t_transform_chain	chain_shear(t_transform_chain chain, t_propo proportion)
{
	t_matrix shear = shearing(proportion);
	chain.matrix = multiply_matrix(shear, chain.matrix);
	return chain;
}

// Get the final matrix from the chain
t_matrix	get_transform(t_transform_chain chain)
{
	return chain.matrix;
}

int main()
{
	t_tuple		point = {1, 0, 1, 1};
	
	// OLD WAY (nested function calls - hard to read)
	t_tuple		result_old = multiply_matrix_by_tuple(translation(10, 5, 7), multiply_matrix_by_tuple(scaling(5, 5, 5), multiply_matrix_by_tuple(rotation_x(M_PI / 2), point)));
	
	// NEW WAY (fluent API - natural reading order)
	t_matrix transform = get_transform(
		chain_translate(
			chain_scale(
				chain_rotate_x(
					identity(), 
					M_PI / 2
				), 
				5, 5, 5
			), 
			10, 5, 7
		)
	);
	
	t_tuple result_new = multiply_matrix_by_tuple(transform, point);
	
	printf("Old way result: %.2f, %.2f, %.2f, %.2f\n", result_old.x, result_old.y, result_old.z, result_old.w);
	printf("New way result: %.2f, %.2f, %.2f, %.2f\n", result_new.x, result_new.y, result_new.z, result_new.w);
	
	// Even more natural way - step by step
	t_transform_chain my_transform = identity();
	my_transform = chain_rotate_x(my_transform, M_PI / 2);
	my_transform = chain_scale(my_transform, 5, 5, 5);
	my_transform = chain_translate(my_transform, 10, 5, 7);
	
	t_tuple result_step = multiply_matrix_by_tuple(get_transform(my_transform), point);
	printf("Step way result: %.2f, %.2f, %.2f, %.2f\n", result_step.x, result_step.y, result_step.z, result_step.w);
	
	// Additional tests to demonstrate the fluent API
	printf("\n=== Additional Fluent API Tests ===\n");
	
	// Complex transformation chain
	t_transform_chain complex = identity();
	complex = chain_rotate_y(complex, M_PI / 4);  // 45 degrees around Y
	complex = chain_scale(complex, 2, 0.5, 1.5);   // Scale X=2, Y=0.5, Z=1.5
	complex = chain_rotate_z(complex, M_PI / 6);   // 30 degrees around Z
	complex = chain_translate(complex, -3, 7, 2);  // Move to (-3, 7, 2)
	
	t_tuple test_point = {1, 1, 1, 1};
	t_tuple complex_result = multiply_matrix_by_tuple(get_transform(complex), test_point);
	printf("Complex chain result: %.2f, %.2f, %.2f, %.2f\n", 
		complex_result.x, complex_result.y, complex_result.z, complex_result.w);
	
	// Test with shearing
	t_propo shear_props = {0.5, 0, 0, 0, 0, 0};  // Shear X in proportion to Y
	t_transform_chain shear_test = identity();
	shear_test = chain_shear(shear_test, shear_props);
	shear_test = chain_translate(shear_test, 1, 1, 1);
	
	t_tuple shear_result = multiply_matrix_by_tuple(get_transform(shear_test), test_point);
	printf("Shear + translate result: %.2f, %.2f, %.2f, %.2f\n", 
		shear_result.x, shear_result.y, shear_result.z, shear_result.w);
	
	return 0;
}