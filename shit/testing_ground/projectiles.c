# include <unistd.h>
# include <float.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
typedef struct s_tuple
{
	float	x;
	float	y;
	float	z;
	float	w;
}			t_tuple;
bool	f_equal(float a, float b);
float	vector_magnitude(t_tuple v);
t_tuple add_tuple(t_tuple a, t_tuple b);
t_tuple	normalizing_vector(t_tuple v);


bool	tuple_comparison(t_tuple a, t_tuple b)
{
	return (f_equal(a.x, b.x) && f_equal(a.y, b.y) && f_equal(a.z, b.z) && a.w == b.w);
}



t_tuple substract_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

t_tuple negate_tuple(t_tuple a)
{
	return (t_tuple){-a.x, -a.y, -a.z, a.w};
}



t_tuple tuple_scalar_divide(t_tuple a, float scalar)
{
	if (f_equal(scalar, 0.0f))
		return(t_tuple){0, 0, 0, a.w};
	return(t_tuple){(a.x / scalar), (a.y / scalar), (a.z / scalar), a.w};
}


//normilization


/* Dot Product is a way of combining two vectors to get a single number
that represent as far as i know the angle between them ??
	a⋅b= ax ​bx ​+ ay ​by ​+ az​ bz​
*/
float	vecs_dot_product(t_tuple va, t_tuple vb)
{
	return ((va.x * vb.x) + (va.y * vb.y) + (va.z * vb.z));
}
/*cross_ product 
	a×b=(ay​bz​−az​by​,az​bx​−ax​bz​,ax​by​−ay​bx​)
*/
t_tuple	vecs_cross_product(t_tuple va, t_tuple vb)
{
	return (t_tuple) {(va.y * vb.z) - (va.z * vb.y) , (va.z * vb.x) - (va.x * vb.z), (va.x * vb.y) - (va.y * vb.x), 0};
}
/* Geometric interpretation
	a⋅b=∣a∣∣b∣cosθ
*/

/*
    •A projectile has a position (a point) and a velocity (a vector).
    •An environment has gravity (a vector) and wind (a vector).
*/

# define EPSILON 1e-6f


bool	f_equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

float	vector_magnitude(t_tuple v)
{
	return (sqrtf((v.x * v.x) + (v.y * v.y) + (v.z *v.z)));
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


t_tuple add_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

projectile  tick(environemt env, projectile proj)
{
    t_tuple new_pos;
    t_tuple new_vec;

    new_pos = add_tuple(proj.point, proj.velocity);
    new_vec = add_tuple(add_tuple(env.wind, env.gravity), proj.velocity);
    return (projectile){new_pos, new_vec};
}


t_tuple	normalizing_vector(t_tuple v)
{
	float mag = vector_magnitude(v);
	if (mag == 0.0f)
        return write(2, "mag == 0", 9),(t_tuple){0,0,0,0};
	return (t_tuple){(v.x / mag), (v.y / mag), (v.z / mag), v.w};
}

t_tuple tuple_scalar_mult(t_tuple a, float scalar)
{
	return(t_tuple){(a.x * scalar), (a.y * scalar), (a.z * scalar), a.w};
}

// int main ()
// {
//     t_tuple velocity = (t_tuple){1, 1, 1, 0};
//     projectile original_proj = {(t_tuple){0, 1, 0, 1}, tuple_scalar_mult(normalizing_vector(velocity), -2.5)};
//     // /e←environment(vector(0, -0.1, 0),vector(-0.01, 0, 0))
//     environemt original_env = {(t_tuple){0, -0.1, 0, 0}, (t_tuple){-0.01, 0, 0, 0}};

//     projectile new_proj = original_proj;
//         printf("x:%f\ty:%f\tz:%f\n",new_proj.point.x, new_proj.point.y, new_proj.point.z);
//     while (new_proj.point.y > 0)
//     {
//         new_proj = tick(original_env, new_proj);
//         printf("x:%f\ty:%f\tz:%f\n",new_proj.point.x, new_proj.point.y, new_proj.point.z);
//     }
// }

// typedef struct s_rgb
// {
// 	uint8_t	red;
// 	uint8_t	green;
// 	uint8_t	blue;
// 	uint8_t	alpha;
// }			t_rgba;

// t_tuple tuple_from_rgba(t_rgba c)
// {
//     return (t_tuple){c.red, c.green, c.blue, c.alpha};
// }

t_tuple Hadamard_product(t_tuple a, t_tuple b)
{
    return (t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, 2};
}

int main ()
{
    t_tuple color_a = {0.9, 0.6, 0.75, 2};
    t_tuple color_b = {0.7, 0.1, 0.25, 2};
    t_tuple new_color = add_tuple(color_a, color_b);
    // printf("r:%.2f g:%.2f b:%.2f\n", new_color.x, new_color.y, new_color.z);
    // new_color = substract_tuple(color_a, color_b);
    // printf("r:%.2f g:%.2f b:%.2f\n", new_color.x, new_color.y, new_color.z);
    // new_color = tuple_scalar_mult((t_tuple){0.2, 0.3, 0.4}, 2);
    // printf("r:%.2f g:%.2f b:%.2f\n", new_color.x, new_color.y, new_color.z);

    new_color = Hadamard_product((t_tuple){1, 0.2, 0.4}, (t_tuple){0.9, 1, 0.1});
    printf("r:%.2f g:%.2f b:%.2f\n", new_color.x, new_color.y, new_color.z);
}
