# include <unistd.h>
# include <float.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "../MLX42/include/MLX42/MLX42.h"
# define EPSILON 1e-6f
#define HEIGHT 550
#define WIDTH  900

typedef struct s_rgb
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	uint8_t	alpha;
}			t_rgba;

typedef struct s_tuple
{
	float	x;
	float	y;
	float	z;
	float	w;
}			t_tuple;

typedef struct s_Ambient_light
{
	float	l_ratio;
	t_tuple	color;
}			t_Ambient_light;

typedef struct s_Camera
{
	t_tuple	coord;
	t_tuple	orient_v;
	int		fov;
}			t_Camera;

typedef struct s_light
{
	t_tuple			coord;
	float			b_ratio;
	t_tuple			color; // bonus only
	struct s_light	*next;
}			t_light;

typedef struct s_sphere
{
	t_tuple	center;
	float	diam;
	t_rgba	color;
}			t_sphere;

typedef struct s_plane
{
	t_tuple	point;
	t_tuple	normal;
	t_rgba	color;
}			t_plane;

typedef struct s_cylinder
{
	t_tuple	center;
	t_tuple	axis;
	float	diameter;
	float	height;
	t_rgba	color;
}			t_cylinder;

typedef enum e_obj_type
{
    OBJ_SPHERE,
    OBJ_PLANE,
    OBJ_CYLINDER
}		t_obj_type;

typedef struct s_object
{
    t_obj_type		type;
    union
    {
        t_sphere    sp;
        t_plane     pl;
        t_cylinder  cy;
    } shape;
    struct s_object	*next;
}   				t_object;

typedef struct s_data
{
	t_Ambient_light	ambl;
	t_Camera		cam;
	t_light			*light;
	t_object		*object;


	mlx_t			*ptr;
	mlx_image_t		*img;
}   				t_data;

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

typedef struct s_anim
{
    projectile proj;
    environemt env;
    mlx_image_t *img;
    t_tuple color;
} t_anim;


typedef struct s_screen
{
	double		x;
	double		y;
}				t_screen;

int convert_color(t_tuple c)
{
    uint8_t r = (uint8_t)(c.x * 255);
    uint8_t g = (uint8_t)(c.y * 255);
    uint8_t b = (uint8_t)(c.z * 255);
    uint8_t a = (uint8_t)(c.w * 255);

    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | a;
}


float	ft_step(t_screen *d)
{
	if (fabs(d->x) > fabs(d->y))
		return (fabs(d->x));
	return (fabs(d->y));
}

void	draw_line(t_screen one, t_screen two, mlx_image_t *img, int color)
{
	t_screen	screen;
	t_screen	d;
	float		step;
	float		i;

	screen.x = one.x;
	screen.y = one.y;
	d.x = two.x - one.x;
	d.y = two.y - one.y;
	step = ft_step(&d);
	d.x /= step;
	d.y /= step;
	i = 0;
	while (i <= step)
	{
		if (screen.x >= 0 && screen.y >= 0 && screen.x < img->width
			&& screen.y < img->height)
			mlx_put_pixel(img, (int)screen.x, (int)screen.y, color);
		screen.x += d.x;
		screen.y += d.y;
		i++;
	}
}


float	vector_magnitude(t_tuple v)
{
	return (sqrtf((v.x * v.x) + (v.y * v.y) + (v.z *v.z)));
}
//normilization

t_tuple	normalizing_vector(t_tuple v)
{
	float mag = vector_magnitude(v);
	if (mag == 0.0f)
        return write(2, "mag == 0", 9),(t_tuple){0,0,0,0};
	return (t_tuple){(v.x / mag), (v.y / mag), (v.z / mag), v.w};
}
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

t_tuple Hadamard_product(t_tuple a, t_tuple b)
{
    return (t_tuple){a.x * b.x, a.y * b.y, a.z * b.z, 2};
}



/* Geometric interpretation
	a⋅b=∣a∣∣b∣cosθ
*/



bool	f_equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

bool	tuple_comparison(t_tuple a, t_tuple b)
{
	return (f_equal(a.x, b.x) && f_equal(a.y, b.y) && f_equal(a.z, b.z) && a.w == b.w);
}

t_tuple add_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

t_tuple substract_tuple(t_tuple a, t_tuple b)
{
	return (t_tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

t_tuple negate_tuple(t_tuple a)
{
	return (t_tuple){-a.x, -a.y, -a.z, a.w};
}

t_tuple tuple_scalar_mult(t_tuple a, float scalar)
{
	return(t_tuple){(a.x * scalar), (a.y * scalar), (a.z * scalar), a.w};
}

t_tuple tuple_scalar_divide(t_tuple a, float scalar)
{
	if (f_equal(scalar, 0.0f))
		return(t_tuple){0, 0, 0, a.w};
	return(t_tuple){(a.x / scalar), (a.y / scalar), (a.z / scalar), a.w};
}


projectile  tick(environemt env, projectile proj)
{
    t_tuple new_pos;
    t_tuple new_vec;

    new_pos = add_tuple(proj.point, proj.velocity);
    new_vec = add_tuple(add_tuple(env.wind, env.gravity), proj.velocity);
    return (projectile){new_pos, new_vec};
}

void update(void *param)
{
    t_anim *a = (t_anim*)param;

    if (a->proj.point.y <= 0 || a->proj.point.x < 0 || a->proj.point.x >= WIDTH)
        return; 

    projectile old = a->proj;
    a->proj = tick(a->env, a->proj);

    int x0 = (int)old.point.x;
    int y0 = HEIGHT - 1 - (int)old.point.y;
    int x1 = (int)a->proj.point.x;
    int y1 = HEIGHT - 1 - (int)a->proj.point.y;

    draw_line((t_screen){x0,y0}, (t_screen){x1,y1}, a->img, convert_color(a->color));
}
#include <math.h>

int main()
{
    t_data data;
    data.ptr = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    data.img = mlx_new_image(data.ptr, WIDTH, HEIGHT);

    t_tuple velocity = {1,1.8,0,0};
    projectile proj = {(t_tuple){0,1,0,1}, tuple_scalar_mult(normalizing_vector(velocity), 11.25)};
    environemt env = {(t_tuple){0,-0.9,0,0}, (t_tuple){0,      0.1,    0,    0}};

    t_anim anim = {proj, env, data.img, (t_tuple){1,0,0,1}};

    mlx_image_to_window(data.ptr, data.img, 0, 0);
	int start_y = HEIGHT / 30;
	int end_y   = HEIGHT / 4 + 75 ;
	int x = WIDTH - 45;  // moved more to the left

	// vertical line of the arrow
	for (int y = start_y; y <= end_y; y += 20)
	{
		mlx_put_string(data.ptr, "|", x, y);
	}

	// labels
	mlx_put_string(data.ptr, "wind<- - - - - - - ", x - 180, start_y);
	mlx_put_string(data.ptr, "gravity\\/", x - 75, end_y);

	// int start_y = HEIGHT / 10;
	// int end_y = HEIGHT / 4;
	// int x = WIDTH / 7;
	// for (int y = start_y; y <= end_y; y += 20) // smaller step → smoother line
	// {
	// 	mlx_put_string(data.ptr, "|", x, y);
	// }
	// mlx_put_string(data.ptr, "<------- wind", WIDTH/8, start_y);
	// mlx_put_string(data.ptr, "\\/ gravity   ", x-5, end_y+10);

    mlx_loop_hook(data.ptr, update, &anim); 
    mlx_loop(data.ptr);
}