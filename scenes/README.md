# miniRT - Ray Tracer

A simple ray tracer implementation in C using the MLX42 graphics library.

## Project Structure

```
mandatory/
├── includes/           # Header files
│   ├── miniRT.h       # Main header with all includes
│   ├── types.h        # All struct and type definitions  
│   ├── core.h         # Core ray tracing function prototypes
│   ├── math_ops.h     # Mathematical operation prototypes
│   ├── geometry.h     # Geometry function prototypes
│   ├── lighting.h     # Lighting function prototypes
│   ├── parsing.h      # Parsing function prototypes
│   └── rendering.h    # Rendering function prototypes
│
├── src/               # Source files (max 5 functions per file)
│   ├── main.c         # Program entry point
│   │
│   ├── core/          # Core ray tracing functionality
│   │   ├── camera.c            # Camera creation and setup
│   │   ├── ray_casting.c       # Ray generation for pixels
│   │   ├── shading.c           # Shading and shadow calculations
│   │   ├── view_transform.c    # View transformation matrix
│   │   ├── world.c             # World setup and intersection
│   │   ├── computations.c      # Pre-computation for intersections
│   │   └── intersections.c     # Intersection list management
│   │
│   ├── math/          # Mathematical operations
│   │   ├── tuple_basic.c           # Basic tuple operations (add, subtract, negate)
│   │   ├── tuple_operations.c      # Extended tuple operations (multiply, divide)
│   │   ├── vector_operations.c     # Vector operations (magnitude, normalize, dot, cross)
│   │   ├── vector_reflection.c     # Vector reflection
│   │   ├── matrix_basic.c          # Basic matrix operations (multiply, transpose)
│   │   ├── matrix_determinant.c    # Matrix determinant calculation
│   │   ├── matrix_inverse.c        # Matrix inversion
│   │   ├── transformations.c       # Translation and scaling transformations
│   │   └── transformations_rotation.c # Rotation transformations
│   │
│   ├── geometry/      # Geometric shapes and intersections
│   │   ├── sphere.c       # Sphere creation and properties
│   │   ├── plane.c        # Plane creation and properties
│   │   ├── cylinder.c     # Cylinder creation and properties
│   │   ├── rays.c         # Ray operations and intersections
│   │   └── normals.c      # Normal vector calculations
│   │
│   ├── lighting/      # Lighting calculations
│   │   ├── phong_lighting.c    # Phong illumination model
│   │   └── light_and_shades.c  # Light setup and shadow utilities
│   │
│   ├── rendering/     # Rendering and image output
│   │   ├── render.c              # Main rendering loop with progress bar
│   │   ├── image_export.c        # PPM image export
│   │   ├── color_conversion.c    # Color format conversion
│   │   └── coordinate_conversion.c # Coordinate system conversion
│   │
│   └── parsing/       # Scene file parsing
│       ├── parsing.c                  # Main parsing coordinator
│       ├── extraction.c               # Legacy extraction (to be removed)
│       ├── validators.c               # Input validation functions
│       ├── field_parsers.c            # Field parsing utilities
│       ├── ambient_camera_extract.c   # Ambient light and camera parsing
│       ├── light_extract.c            # Light source parsing
│       ├── sphere_extract.c           # Sphere object parsing
│       ├── plane_extract.c            # Plane object parsing
│       ├── cylinder_extract.c         # Cylinder object parsing
│       ├── scene_extractors.c         # Scene extraction coordinator
│       ├── split.c                    # String splitting utility
│       ├── apply_ambient.c            # Apply ambient to objects
│       └── debug_parsing.c            # Debug and validation output
│
├── obj/               # Compiled object files (generated)
├── maps/              # Scene description files (.rt format)
├── libft/             # Custom C library
├── get_next_line/     # Line reading utility
└── MLX42/             # Graphics library

```

## Function Documentation

### Core Functions

#### camera.c
- `t_camera camera(double hsize, double vsize, double fov)` - Creates a camera with specified dimensions and field of view
- `t_matrix orientation(t_tuple left, t_tuple up, t_tuple forward)` - Creates orientation matrix from basis vectors

#### ray_casting.c  
- `t_ray ray_for_pixel(t_camera cam, double px, double py)` - Generates a ray through a specific pixel

#### shading.c
- `bool is_shadowed(t_world world, t_tuple point, t_light *light)` - Checks if a point is in shadow
- `t_tuple shade_hit(t_world world, t_comps *comp)` - Calculates color at an intersection point
- `t_tuple color_at(t_world world, t_ray ray)` - Calculates color for a ray in the world

#### view_transform.c
- `t_matrix view_transformation(t_tuple from, t_tuple to, t_tuple up)` - Creates view transformation matrix

#### world.c
- `t_world new_world(void)` - Creates a new empty world
- `t_inters *intersect_world(t_world world, t_ray ray)` - Finds all intersections of a ray with world objects
- `t_object *new_object(t_obj_type type, t_shapes shape)` - Creates a new object
- `t_light *new_light(t_tuple point, t_tuple color)` - Creates a new light source

#### computations.c
- `t_comps *prepare_computations(t_inters *intersection, t_ray ray)` - Precomputes intersection data

#### intersections.c
- `t_inters *new_intersection(float t, t_object *obj)` - Creates new intersection
- `void ft_add_intersection(t_inters **lst, t_inters *new)` - Adds intersection to list
- `void sort_intersections(t_inters **lst)` - Sorts intersections by t value
- `float hit(t_inters *list)` - Finds closest positive intersection

### Math Functions

#### tuple_basic.c
- `bool f_equal(float a, float b)` - Floating point equality comparison
- `bool tuple_comparison(t_tuple a, t_tuple b)` - Tuple equality comparison
- `t_tuple add_tuple(t_tuple a, t_tuple b)` - Adds two tuples
- `t_tuple substract_tuple(t_tuple a, t_tuple b)` - Subtracts two tuples
- `t_tuple negate_tuple(t_tuple a)` - Negates a tuple

#### tuple_operations.c
- `t_tuple tuple_scalar_mult(t_tuple a, float scalar)` - Multiplies tuple by scalar
- `t_tuple tuple_scalar_divide(t_tuple a, float scalar)` - Divides tuple by scalar
- `t_tuple mult_tuple(t_tuple a, t_tuple b)` - Element-wise tuple multiplication

#### vector_operations.c
- `float vector_magnitude(t_tuple v)` - Calculates vector magnitude
- `t_tuple normalizing_vector(t_tuple v)` - Normalizes a vector
- `float vecs_dot_product(t_tuple a, t_tuple b)` - Calculates dot product
- `t_tuple vecs_cross_product(t_tuple a, t_tuple b)` - Calculates cross product
- `t_tuple Hadamard_product(t_tuple a, t_tuple b)` - Hadamard (element-wise) product

#### vector_reflection.c
- `t_tuple reflect(t_tuple in, t_tuple normal)` - Reflects vector around normal

#### matrix_basic.c
- `t_matrix identity(void)` - Creates 4x4 identity matrix
- `bool compare_matrix(t_matrix a, t_matrix b)` - Compares two matrices
- `t_matrix multiply_matrix(t_matrix a, t_matrix b)` - Multiplies two matrices
- `t_tuple multiply_matrix_by_tuple(t_matrix m, t_tuple t)` - Multiplies matrix by tuple
- `t_matrix transposing_matrix(t_matrix m)` - Transposes a matrix

#### matrix_determinant.c
- `float matrix_determinant_2x2(t_matrix m)` - Calculates 2x2 determinant
- `t_matrix get_sub_matrix(t_matrix m, int row, int col)` - Gets submatrix
- `float get_matrix_minor_3x3(t_matrix m, int row, int col)` - Calculates 3x3 minor
- `float compute_cofactor(t_matrix m, int row, int col)` - Computes cofactor
- `float matrix_determinant(t_matrix m)` - Calculates matrix determinant

#### matrix_inverse.c
- `bool check_if_invertible(t_matrix m)` - Checks if matrix is invertible
- `t_matrix inverse_matrix(t_matrix m)` - Inverts a matrix

#### transformations.c
- `t_matrix translation(float x, float y, float z)` - Creates translation matrix
- `t_matrix scaling(float x, float y, float z)` - Creates scaling matrix
- `double to_radians(int degrees)` - Converts degrees to radians
- `t_matrix shearing(t_propo prop)` - Creates shearing matrix

#### transformations_rotation.c
- `t_matrix rotation_x(float theta)` - Creates X-axis rotation matrix
- `t_matrix rotation_y(float theta)` - Creates Y-axis rotation matrix
- `t_matrix rotation_z(float theta)` - Creates Z-axis rotation matrix

### Geometry Functions

#### rays.c
- `t_ray transform_ray(t_ray ray, t_matrix m)` - Transforms a ray by a matrix
- `t_tuple position(t_ray ray, float distance)` - Calculates position along ray

#### sphere.c
- `t_sphere sphere(void)` - Creates a unit sphere at origin
- `t_material material(void)` - Creates default material
- `t_sphere sp_transform(t_matrix m, t_sphere sp)` - Applies transformation to sphere
- `bool intersect_sphere(t_sphere *sp, t_ray ray, t_inters **intersections)` - Finds ray-sphere intersections
- `t_tuple sphere_normal_at(t_sphere sp, t_tuple point)` - Calculates normal at point on sphere

#### plane.c
- `t_plane plane(void)` - Creates a plane at y=0
- `t_tuple plane_normal_at(t_plane pl)` - Gets plane normal (always up)
- `bool intersect_plane(t_plane *pl, t_ray ray, t_inters **intersections)` - Finds ray-plane intersections

#### cylinder.c
- `t_cylinder cylinder(void)` - Creates a cylinder
- `bool intersect_cylinder(t_cylinder *cyl, t_ray ray, t_inters **intersections)` - Finds ray-cylinder intersections
- `t_tuple cylinder_normal_at(t_cylinder cyl, t_tuple point)` - Calculates normal at point on cylinder

#### normals.c
- `t_tuple normal_at(t_object *obj, t_tuple world_point)` - Calculates normal for any object type

### Lighting Functions

#### phong_lighting.c
- `t_light point_light(t_tuple position, t_tuple intensity)` - Creates a point light
- `t_tuple lighting(t_material m, t_light l, t_tuple point, t_tuple eyev, t_tuple normalv, bool shadow, t_tuple ambient)` - Calculates Phong illumination

### Rendering Functions

#### render.c
- `void print_progress_bar(int current, int total, int width)` - Displays rendering progress
- `mlx_image_t *render(mlx_t *mlx, t_camera cam, t_world world)` - Main rendering loop

#### image_export.c
- `bool mlx_image_to_ppm(mlx_image_t *img, char *filename, unsigned int range)` - Exports image to PPM format

#### color_conversion.c
- `int convert_color(t_tuple c)` - Converts tuple color to integer format

### Parsing Functions

#### parsing.c
- `bool check_extension(char *file)` - Validates .rt file extension
- `bool complete_parsing(char *file, t_data *data)` - Main parsing coordinator
- `void errors_map(int code)` - Displays error messages

#### validators.c
- `bool i_field_validation(char *str)` - Validates integer field
- `bool f_field_validation(char *str)` - Validates float field
- `bool i_range_validator(int min, int max, char *line)` - Validates integer range
- `bool f_range_validator(float min, float max, char *line)` - Validates float range

#### field_parsers.c
- `float ft_atof(char *str)` - Converts string to float
- `void skip_spaces(char *line, int *i)` - Skips whitespace
- `bool check_fields_num(char **fields, int n)` - Validates field count
- `bool tuple_validator(char **fields, bool b, float min, float max)` - Validates tuple fields
- `void free_matrix(char **matrix)` - Frees string array

#### ambient_camera_extract.c
- `bool extract_ambient_light(char *line, t_data *data, t_parser *parser)` - Parses ambient light
- `bool extract_camera(char *line, t_data *data, t_parser *parser)` - Parses camera

#### light_extract.c
- `bool extract_light(char *line, t_data *data, t_parser *parser)` - Parses light source

#### sphere_extract.c
- `bool sphere_extract(char *line, t_data *data)` - Parses sphere object

#### plane_extract.c
- `bool plane_extract(char *line, t_data *data)` - Parses plane object

#### cylinder_extract.c
- `bool cylinder_extract(char *line, t_data *data)` - Parses cylinder object

#### scene_extractors.c
- `bool extractor(char *line, t_data *data, t_parser *parser)` - Routes line to appropriate extractor
- `bool extract_data(char *file, t_data *data)` - Main extraction loop

#### split.c
- `char **ft_split_rt(const char *s, char *charset)` - Splits string by delimiters

#### apply_ambient.c
- `void apply_ambient_to_objects(t_data *data)` - Applies ambient ratio to all objects

#### debug_parsing.c
- `void check_parsed_data(t_data *data)` - Validates parsed data
- `void print_matrix(const char *label, t_matrix m)` - Prints matrix for debugging
- `void debug_world_state(t_world world, t_camera cam)` - Prints world state for debugging

## Scene File Format (.rt)

The scene file describes the 3D environment using a simple text format:

### Ambient Lighting
```
A <ratio> <R,G,B>
```
- `ratio`: Ambient light intensity (0.0 to 1.0)
- `R,G,B`: Ambient color (0-255 per channel)

Example: `A 0.2 255,255,255`

### Camera
```
C <x,y,z> <nx,ny,nz> <fov>
```
- `x,y,z`: Camera position
- `nx,ny,nz`: Orientation vector (normalized)
- `fov`: Field of view in degrees (0-180)

Example: `C 0,0,10 0,0,-1 70`

### Light
```
L <x,y,z> <brightness> <R,G,B>
```
- `x,y,z`: Light position
- `brightness`: Light intensity (0.0 to 1.0)
- `R,G,B`: Light color (0-255 per channel)

Example: `L 10,10,10 0.7 255,255,255`

### Sphere
```
sp <x,y,z> <diameter> <R,G,B>
```
- `x,y,z`: Center position
- `diameter`: Sphere diameter
- `R,G,B`: Surface color (0-255 per channel)

Example: `sp 0,0,0 2 255,0,0`

### Plane
```
pl <x,y,z> <nx,ny,nz> <R,G,B>
```
- `x,y,z`: Point on plane
- `nx,ny,nz`: Normal vector (normalized)
- `R,G,B`: Surface color (0-255 per channel)

Example: `pl 0,0,0 0,1,0 200,200,200`

### Cylinder
```
cy <x,y,z> <nx,ny,nz> <diameter> <height> <R,G,B>
```
- `x,y,z`: Center position
- `nx,ny,nz`: Axis orientation (normalized)
- `diameter`: Cylinder diameter
- `height`: Cylinder height
- `R,G,B`: Surface color (0-255 per channel)

Example: `cy 0,0,0 0,1,0 1 2 0,255,0`

## Compilation

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile everything
```

## Usage

```bash
./miniRT <scene_file.rt>
```

Example:
```bash
./miniRT maps/minimal.rt
```

### Controls
- `T` key: Export current view to `output.ppm`
- Close window or `ESC`: Exit program

## Dependencies

- MLX42 graphics library
- libft (custom C library)
- get_next_line (file reading utility)
- OpenGL, GLFW, X11 (for MLX42)

## Features

- Ray tracing with Phong illumination model
- Multiple light sources support
- Shadow calculation
- Three geometric primitives: Sphere, Plane, Cylinder
- Transformation matrices (translation, rotation, scaling)
- Camera positioning and orientation
- Ambient lighting
- PPM image export
- Real-time rendering with progress bar
- Scene file parsing with validation

## Implementation Notes

### Coordinate System
- Right-handed coordinate system
- Y-axis points up
- Z-axis points toward the viewer
- Objects are defined in object space (centered at origin)
- Transformations handle world positioning

### Color Model
- Colors stored as tuples (x=R, y=G, z=B, w=0)
- Internal range: 0.0 to 1.0
- Scene file range: 0 to 255
- Automatic clamping prevents overflow

### Performance
- 1920x1080 default resolution
- Progress bar shows rendering status
- Multi-light scene handling
- Efficient intersection algorithms

## Known Limitations

- No reflections or refractions
- No textures
- Basic anti-aliasing
- Single-threaded rendering

## Author

amn700 / amn

## License

See 42 School guidelines
