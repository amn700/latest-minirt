#include "../../includes/miniRT_bonus.h"

// Forward declaration
float matrix_determinant(t_matrix matrix);

bool    compare_matrix(t_matrix a, t_matrix b)
{
	int i = 0;
	while (i < a.size)
	{
		int j = 0;
		while (j < b.size)
		{
			if (!f_equal(a.mtrx[i][j], b.mtrx[i][j]))
				return false;
			j++;
		}
		i++;
	}
	return true;
}

t_matrix multiply_matrix(t_matrix a, t_matrix b)
{
	t_matrix new;
	new.size = a.size;
	
	int i = 0;
	while (i < a.size)
	{
		int j = 0;
		while (j < b.size)
		{
			new.mtrx[i][j] = 0;
			int k = 0;
			while (k < a.size)
			{
				new.mtrx[i][j] += a.mtrx[i][k] * b.mtrx[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return new;
}

t_tuple	multiply_matrix_by_tuple(t_matrix m, t_tuple v)
{
	t_matrix	new;

	new.mtrx[0][0] = v.x;
	new.mtrx[1][0] = v.y;
	new.mtrx[2][0] = v.z;
	new.mtrx[3][0] = v.w;
	new.size = 4;
	new = multiply_matrix(m, new);
	return (t_tuple){new.mtrx[0][0], new.mtrx[1][0], new.mtrx[2][0], new.mtrx[3][0]};
}


t_matrix transposing_matrix(t_matrix matrix)
{
	t_matrix new;
	new.size = matrix.size;
	int i = 0;
	int j = 0;
	while (i < matrix.size) // rows
	{
		j = 0;
		while (j < matrix.size) // cols
		{
			new.mtrx[i][j] = matrix.mtrx[j][i];
			j++;
		}
		i++;
	}
	return new;
}

// determinant[a  b, c  d]=ad−bc
float	matrix_determinant_2x2(t_matrix matrix)
{
	return (float) (matrix.mtrx[0][0] * matrix.mtrx[1][1]) - (matrix.mtrx[1][0] * matrix.mtrx[0][1]);
}


t_matrix get_sub_matrix(t_matrix matrix, int row, int col)
{
    t_matrix new;
    new.size = matrix.size - 1;

    int c_row = 0;
    int n_row = 0;
    while (c_row < matrix.size)
    {
        if (c_row != row)
        {
            int c_col = 0;
            int n_col = 0;
            while (c_col < matrix.size)
            {
                if (c_col != col)
                {
                    new.mtrx[n_row][n_col] = matrix.mtrx[c_row][c_col];
                    n_col++;
                }
                c_col++;
            }
            n_row++;
        }
        c_row++;
    }
    return new;
}

float	get_matrix_minor_3x3(t_matrix matrix, int row, int col)
{
	t_matrix sub = get_sub_matrix(matrix, row, col);
	return matrix_determinant_2x2(sub);
}


float compute_cofactor(t_matrix matrix, int row, int col)
{
    t_matrix sub = get_sub_matrix(matrix, row, col);
    float minor = matrix_determinant(sub);
    if ((row + col) % 2 == 0)
        return minor;
    return -minor;
}


float matrix_determinant(t_matrix matrix)
{
	float deter = 0;
	if (matrix.size == 2)
		deter = matrix_determinant_2x2(matrix);
	else
	{
		int col = 0;
		while (col < matrix.size)
		{
			float cofactor = compute_cofactor(matrix, 0, col);
			deter = deter + (matrix.mtrx[0][col] * cofactor);
			col++;		
		}
	}
	return deter;
}

bool	check_if_invertible(t_matrix matrix)
{
	return (matrix_determinant(matrix));	
}

t_matrix	inverse_matrix(t_matrix matrix)
{
	t_matrix inverted;
	inverted.size = 4;
	float orginal_deter = matrix_determinant(matrix);
	int row = 0;
	while (row < matrix.size)
	{
		int col = 0;
		while (col < matrix.size)
		{
			inverted.mtrx[col][row] = compute_cofactor(matrix, row, col) / orginal_deter;
			col++;
		}
		row++;
	}
	return inverted;
}

// float matrix_determinant_3x3(t_matrix matrix)
// {
// 	float first = compute_cofactor(matrix, 0, 0)  * matrix.mtrx[0][0];
// 	float second = compute_cofactor(matrix, 0, 1) * matrix.mtrx[0][1];
// 	float third = compute_cofactor(matrix, 0, 2)  * matrix.mtrx[0][2];
// 	return (first + second + third);
// }

// int main ()
// {
	// t_tuple point = {-4, 6, 8, 0};
	
	// t_matrix transformation = scaling(2, 3, 4);
	// t_tuple result = multiply_matrix_by_tuple(transformation, point);
	// t_matrix inverse = inverse_matrix(transformation);
	
	// t_tuple result = multiply_matrix_by_tuple
	// (transformation, vector);
	
	// printf("%.2f, %.2f, %.2f, %.2f\n", result.x, result.y, result.z, result.w);
	
	// test_modified_identity_matrix();
	// t_matrix A;

	// A.size = 4;
	// A.mtrx[0][0] = 3;
	// A.mtrx[0][1] = -9;
	// A.mtrx[0][2] = 7;
	// A.mtrx[0][3] = 3;
	
	// A.mtrx[1][0] = 3;
	// A.mtrx[1][1] = -8;
	// A.mtrx[1][2] = 2;
	// A.mtrx[1][3] = -9;
	
	// A.mtrx[2][0] = -4;
	// A.mtrx[2][1] =  4;
	// A.mtrx[2][2] =  4;
	// A.mtrx[2][3] =  1;
	
	// A.mtrx[3][0] = -6;
	// A.mtrx[3][1] =  5;
	// A.mtrx[3][2] = -1;
	// A.mtrx[3][3] = 1;


	// t_matrix B;

	// B.size = 4;
	// B.mtrx[0][0] = 8;
	// B.mtrx[0][1] = 2;
	// B.mtrx[0][2] = 2;
	// B.mtrx[0][3] = 2;
	
	// B.mtrx[1][0] = 3;
	// B.mtrx[1][1] = -1;
	// B.mtrx[1][2] = 7;
	// B.mtrx[1][3] = 0;
	
	// B.mtrx[2][0] = 7;
	// B.mtrx[2][1] =  0;
	// B.mtrx[2][2] =  5;
	// B.mtrx[2][3] =  4;
	
	// B.mtrx[3][0] = 6;
	// B.mtrx[3][1] =  -2;
	// B.mtrx[3][2] = 0;
	// B.mtrx[3][3] = 5;

	// t_matrix C = multiply_matrix(A, B);
	// t_matrix inverse_B = inverse_matrix(B);

	// t_matrix NEW_A = multiply_matrix(C, inverse_B);
	// // if (check_if_invertible(m))
	// // 	printf("matrix is invertible\n");
	// // else
	// // 	printf("matrix is not  invertible\n");
	// // t_matrix inverted = inverse_matrix(m);

	// int row = 0;
	// while (row < A.size)
	// {
	// 	int col = 0;
	// 	while (col < A.size)
	// 	{
	// 		printf("[%.5f]", A.mtrx[row][col]);
	// 		col++;
	// 	}
	// 		printf("\n");
	// 	row++;
	// }
	// row = 0;
	// while (row < NEW_A.size)
	// {
	// 	int col = 0;
	// 	while (col < NEW_A.size)
	// 	{
	// 		printf("[%.5f]", NEW_A.mtrx[row][col]);
	// 		col++;
	// 	}
	// 		printf("\n");
	// 	row++;
	// }
	// printf("%.2f\n", matrix_determinant(m));
	
	// printf("%.2f\n", compute_cofactor_3x3(m, 0, 0));
	// printf("%.2f\n", compute_cofactor_3x3(m, 0, 1));
	// printf("%.2f\n", compute_cofactor_3x3(m, 0, 2));
	
	// printf("%.2f\n", compute_cofactor_3x3(m, 1, 0) * m.mtrx[1][0]);
	// printf("%.2f\n", compute_cofactor_3x3(m, 1, 1) * m.mtrx[1][1]);
	// printf("%.2f\n", compute_cofactor_3x3(m, 1, 2)* m.mtrx[1][2]);
// }
// 	// for (int i = 0; i < new.size; i++)
// 	// {
// 	// 	for (int j = 0; j < new.size; j++)
// 	// 	{
// 	// 		printf("%.2f ", new.mtrx[i][j]);
// 	// 	}
// 	// 	printf("\n");
// 	// }
// }
// int main ()
// {
// 	t_matrix m;
// 	t_matrix ident;
// 	m.size = 4;

// 	ident.size = 4;
// 	ident.mtrx[0][0] = 1;
// 	ident.mtrx[1][0] = 0;
// 	ident.mtrx[2][0] = 0;
// 	ident.mtrx[3][0] = 0;
	
// 	ident.mtrx[0][1] = 0;
// 	ident.mtrx[1][1] = 1;
// 	ident.mtrx[2][1] = 0;
// 	ident.mtrx[3][1] = 0;
	
// 	ident.mtrx[0][2] = 0;
// 	ident.mtrx[1][2] = 0;
// 	ident.mtrx[2][2] = 1;
// 	ident.mtrx[3][2] = 0;
	
// 	ident.mtrx[0][3] = 0;
// 	ident.mtrx[1][3] = 0;
// 	ident.mtrx[2][3] = 0;
// 	ident.mtrx[3][3] = 1;
	
// 	m.mtrx[0][0] = 0;
// 	m.mtrx[1][0] = 1;
// 	m.mtrx[2][0] = 2;
// 	m.mtrx[3][0] = 4;
	
// 	m.mtrx[0][1] = 1;
// 	m.mtrx[1][1] = 2;
// 	m.mtrx[2][1] = 4;
// 	m.mtrx[3][1] = 8;
	
// 	m.mtrx[0][2] = 2;
// 	m.mtrx[1][2] = 4;
// 	m.mtrx[2][2] = 8;
// 	m.mtrx[3][2] = 16;
	
// 	m.mtrx[0][3] = 4;
// 	m.mtrx[1][3] = 8;
// 	m.mtrx[2][3] = 16;
// 	m.mtrx[3][3] = 32;

// 	t_matrix new = multiply_matrix(m, ident);
// 	for (int i = 0; i < m.size; i++)
// 	{
// 		for (int j = 0; j < ident.size; j++)
// 		{
// 			printf("[%f]", new.mtrx[i][j]);
// 		}
// 			printf("\n");
// 	}
// }