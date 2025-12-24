#include "../../includes/miniRT_bonus.h"



/*
[How X transforms] [Where to move X]
[How Y transforms] [Where to move Y]
[How Z transforms] [Where to move Z]
[Always [0 0 0 1]] [Always 1]

[Xx  Xy  Xz  Tx]
[Yx  Yy  Yz  Ty]
[Zx  Zy  Zz  Tz]
[0   0   0   1 ]

*/
t_matrix	translation(float x, float y, float z)
{ 
	t_matrix new;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[0][3] = x;
	new.mtrx[1][3] = y;
	new.mtrx[2][3] = z;
	new.size = 4;
	return new;
}

t_matrix	scaling(float x, float y, float z)
{
t_matrix new;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[0][0] = x;
	new.mtrx[1][1] = y;
	new.mtrx[2][2] = z;
	new.size = 4;
	return new;
}

double	to_radians(int degrees)
{
	return (degrees * M_PI / 180.0);
}

t_matrix	rotation_x(float theta)
{
	t_matrix new;
	new.size = 4;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[1][1] = cos(theta);
	new.mtrx[1][2] = -sin(theta);
	new.mtrx[2][1] = sin(theta);
	new.mtrx[2][2] = cos(theta);
	return new;
}

t_matrix	rotation_y(float theta)
{
	t_matrix new;
	new.size = 4;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[0][0] = cos(theta);
	new.mtrx[0][2] = sin(theta);
	new.mtrx[2][0] = -sin(theta);
	new.mtrx[2][2] = cos(theta);
	return new;
}

t_matrix	rotation_z(float theta)
{
	t_matrix new;
	new.size = 4;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[0][0] = cos(theta);
	new.mtrx[0][1] = -sin(theta);
	new.mtrx[1][0] = sin(theta);
	new.mtrx[1][1] = cos(theta);
	return new;
}

// typedef struct s_propo
// {
// 	float	xy;
// 	float	xz;
// 	float	yx;
// 	float	yz;
// 	float	zx;
// 	float	zy;
// }	t_propo;

t_matrix	shearing(t_propo propotion)
{
	t_matrix new;
	new.size = 4;
	int row = 0;
	while (row < 4)
	{
		int col = 0;
		while (col < 4)
		{
			new.mtrx[row][col] = 0;
			if (col == row)
				new.mtrx[row][col] = 1;
			col++;
		}
		row++;
	}
	new.mtrx[0][1] = propotion.xy;
	new.mtrx[0][2] = propotion.xz;
	new.mtrx[1][0] = propotion.yx;
	new.mtrx[1][2] = propotion.yz;
	new.mtrx[2][0] = propotion.zx;
	new.mtrx[2][1] = propotion.zy;
	return new;
}