# 🔬 miniRT Technical Architecture Guide

A comprehensive deep-dive into how the miniRT ray tracer works, from parsing scene files to rendering the final image.

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Architecture Diagram](#architecture-diagram)
3. [Parsing Pipeline](#parsing-pipeline)
4. [Coordinate Systems](#coordinate-systems)
5. [Transformation Matrices](#transformation-matrices)
6. [Ray Generation](#ray-generation)
7. [Intersection Testing](#intersection-testing)
8. [Shading & Lighting](#shading--lighting)
9. [Shadows](#shadows)
10. [Reflection](#reflection)
11. [Refraction](#refraction)
12. [Textures & UV Mapping](#textures--uv-mapping)
13. [Bump Mapping](#bump-mapping)
14. [Patterns](#patterns)
15. [Rendering Pipeline](#rendering-pipeline)

---

## 🎯 Project Overview

miniRT is a ray tracer that simulates light behavior to generate photorealistic images. The process:

1. **Parse** scene file (.rt) → Extract objects, lights, camera
2. **Setup** world with transformed objects
3. **Render** by casting rays from camera through each pixel
4. **Shade** by calculating light interactions at hit points
5. **Output** final image to window/file

### Core Data Structures

```c
// 4D Tuple (points and vectors)
typedef struct s_tuple {
    float x, y, z, w;  // w=1 for points, w=0 for vectors
} t_tuple;

// Ray (origin + direction)
typedef struct s_ray {
    t_tuple origin;     // Starting point (w=1)
    t_tuple direction;  // Direction vector (w=0)
} t_ray;

// Intersection record
typedef struct s_inters {
    float t;            // Distance along ray
    t_object *obj;      // Hit object
    struct s_inters *next;
} t_inters;
```

---

## 🏗️ Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        SCENE FILE (.rt)                         │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                      PARSING PIPELINE                            │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐        │
│  │ Tokenize │→ │ Validate │→ │ Extract  │→ │ Build    │        │
│  │ Lines    │  │ Fields   │  │ Values   │  │ Objects  │        │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘        │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                         WORLD SETUP                              │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐              │
│  │ Objects     │  │ Lights      │  │ Camera      │              │
│  │ (linked     │  │ (linked     │  │ (view       │              │
│  │  list)      │  │  list)      │  │  transform) │              │
│  └─────────────┘  └─────────────┘  └─────────────┘              │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                      RENDERING LOOP                              │
│  For each pixel (x, y):                                          │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │ 1. ray_for_pixel(camera, x, y) → Generate primary ray    │   │
│  │ 2. color_at(world, ray, depth) → Trace ray               │   │
│  │    ├─ intersect_world() → Find closest hit               │   │
│  │    ├─ prepare_computations() → Hit point details         │   │
│  │    └─ shade_hit() → Calculate final color                │   │
│  │        ├─ lighting() → Phong shading                     │   │
│  │        ├─ is_shadowed() → Shadow test                    │   │
│  │        ├─ reflected_color() → Recursive reflection       │   │
│  │        └─ refracted_color() → Recursive refraction       │   │
│  │ 3. put_pixel(x, y, color) → Write to framebuffer         │   │
│  └──────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                        OUTPUT IMAGE                              │
│            MLX42 Window / PPM File Export                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📖 Parsing Pipeline

### Flow
```
scene.rt → ft_split_rt() → field validation → extract values → build objects
```

### Scene Extractor (`scene_extractors_bonus.c`)
```c
bool extractor(char *line, t_data *data, t_parser *parser)
{
    // Skip empty lines and comments
    if (line[i] == '\0' || line[i] == '#')
        return (true);
    
    // Route to appropriate extractor
    if (ft_strncmp(line, "A ", 2) == 0)
        return extract_ambient_light(line, data, parser);
    if (ft_strncmp(line, "C ", 2) == 0)
        return extract_camera(line, data, parser);
    // ... etc
}
```

### Field Splitting
The `ft_split_rt(line, " \t,\n")` splits on:
- Spaces and tabs (field separators)
- Commas (within coordinates/colors)
- Newlines

Example: `sp 0,2,0 3.0 200,100,50`
→ `["sp", "0", "2", "0", "3.0", "200", "100", "50"]`

### Validation Functions
```c
// Integer field validation
bool i_field_validation(char *str);    // Is it a valid integer?
bool i_range_validator(int min, int max, char *str);  // Is it in range?

// Float field validation  
bool f_field_validation(char *str);    // Is it a valid float?
bool f_range_validator(float min, float max, char *str);

// Tuple validation (3 consecutive fields)
bool tuple_validator(char **fields, bool is_color, float min, float max);
```

### Material Parameter Parsing
Bonus objects can have extended material parameters:
```c
bool parse_material_params(char **fields, int start_idx, t_material *mat, mlx_t *mlx)
{
    while (fields[i]) {
        if (parse_pattern_type(fields[i], mat))        // pat:stripe
        else if (parse_pattern_color(fields[i], ...))  // pa:R:G:B
        else if (parse_reflection(fields[i], ...))     // ref:0.5
        else if (parse_refraction(fields[i], ...))     // refr:0.5:1.5
        else if (parse_shininess(fields[i], ...))      // shine:0.9:256
        else if (texture/bump/normal params)           // texture:path.png
        else return (false);  // Unknown parameter
        i++;
    }
}
```

---

## 🌐 Coordinate Systems

### World Space
- Global 3D coordinate system
- All objects positioned here
- Right-handed: +X right, +Y up, +Z toward viewer

### Object Space (Local Space)
- Each object has its own local coordinates
- Unit objects centered at origin:
  - Unit sphere: center (0,0,0), radius 1
  - Unit cylinder: along Y-axis, radius 1, height 1
  - Unit plane: XZ plane at Y=0
  - Unit cone: tip at origin, along Y-axis

### Camera Space (View Space)
- Camera at origin looking down -Z
- Used for ray generation

### Transformations Flow
```
Object Space → [Object Transform] → World Space → [View Transform] → Camera Space
```

---

## 🔢 Transformation Matrices

4x4 matrices transform points and vectors between coordinate spaces.

### Basic Transforms

#### Translation
```c
t_matrix translation(float x, float y, float z)
{
    // Moves points, doesn't affect vectors (w=0 ignored)
    | 1  0  0  x |
    | 0  1  0  y |
    | 0  0  1  z |
    | 0  0  0  1 |
}
```

#### Scaling
```c
t_matrix scaling(float x, float y, float z)
{
    // Scales along each axis
    | x  0  0  0 |
    | 0  y  0  0 |
    | 0  0  z  0 |
    | 0  0  0  1 |
}
```

#### Rotation (around axes)
```c
t_matrix rotation_x(float radians);  // Rotate around X axis
t_matrix rotation_y(float radians);  // Rotate around Y axis
t_matrix rotation_z(float radians);  // Rotate around Z axis
```

### Object Transform Construction

For a sphere at position (3, 2, 1) with diameter 4:
```c
radius = diameter / 2.0;  // = 2.0
sp.trans = multiply_matrix(
    translation(3, 2, 1),      // Move to position
    scaling(radius, radius, radius)  // Scale to size
);
```

For a cylinder with arbitrary axis:
```c
cl.trans = multiply_matrix(
    translation(x, y, z),
    multiply_matrix(
        align_y_to_vector(axis),  // Rotate Y-axis to match cylinder axis
        scaling(radius, 1, radius)
    )
);
```

### View Transform (Camera)
```c
t_matrix view_transform(t_tuple from, t_tuple to, t_tuple up)
{
    // Creates matrix to transform world → camera space
    forward = normalize(to - from);
    left = cross(forward, normalize(up));
    true_up = cross(left, forward);
    
    orientation = | left.x     left.y     left.z     0 |
                  | true_up.x  true_up.y  true_up.z  0 |
                  | -forward.x -forward.y -forward.z 0 |
                  | 0          0          0          1 |
    
    return multiply_matrix(orientation, translation(-from.x, -from.y, -from.z));
}
```

---

## 📷 Ray Generation

### Camera Setup
```c
t_camera camera(double hsize, double vsize, double fov)
{
    cam.h_view = hsize;       // Horizontal pixels (1920)
    cam.v_view = vsize;       // Vertical pixels (1080)
    cam.fov = fov;            // Field of view in radians
    
    half_view = tan(fov / 2);
    aspect = hsize / vsize;
    
    if (aspect >= 1) {
        cam.half_width = half_view;
        cam.half_height = half_view / aspect;
    } else {
        cam.half_width = half_view * aspect;
        cam.half_height = half_view;
    }
    
    cam.pixel_size = (cam.half_width * 2) / hsize;
}
```

### Ray for Pixel
```c
t_ray ray_for_pixel(t_camera cam, double px, double py)
{
    // Offset from edge of canvas to pixel center
    xoffset = (px + 0.5) * cam.pixel_size;
    yoffset = (py + 0.5) * cam.pixel_size;
    
    // Untransformed world coordinates (canvas at z = -1)
    world_x = cam.half_width - xoffset;
    world_y = cam.half_height - yoffset;
    
    // Transform pixel and origin from camera space to world space
    inv = inverse_matrix(cam.transform);
    pixel = multiply_matrix_by_tuple(inv, point(world_x, world_y, -1));
    origin = multiply_matrix_by_tuple(inv, point(0, 0, 0));
    
    direction = normalize(pixel - origin);
    return ray(origin, direction);
}
```

### Illustration
```
        Canvas (z = -1 in camera space)
        ┌────────────────────────────┐
        │  (px, py)                  │
        │    ↓                       │
        │    ●───────────────────────┼──→ Ray direction
        │                            │
        └────────────────────────────┘
                    ↑
              Camera origin (0,0,0)
```

---

## 🎯 Intersection Testing

### Ray-Object Intersection Flow
```c
t_inters *intersect_world(t_world world, t_ray ray)
{
    for each object in world:
        // Transform ray to object's local space
        transformed_ray = transform_ray(ray, inverse(object.transform));
        
        // Test intersection with unit shape
        intersect_shape(object, transformed_ray, &intersections);
    
    sort_intersections(&intersections);  // By t value
    return intersections;
}
```

### Sphere Intersection
```c
// Unit sphere at origin, radius 1
void intersect_sphere(t_sphere *sp, t_ray ray, t_inters **list, t_object *obj)
{
    // Vector from sphere center to ray origin
    sphere_to_ray = ray.origin - point(0, 0, 0);
    
    // Quadratic coefficients: at² + bt + c = 0
    a = dot(ray.direction, ray.direction);
    b = 2 * dot(ray.direction, sphere_to_ray);
    c = dot(sphere_to_ray, sphere_to_ray) - 1;  // -1 for unit sphere
    
    discriminant = b² - 4ac;
    
    if (discriminant < 0)
        return;  // No intersection
    
    t1 = (-b - sqrt(discriminant)) / (2a);
    t2 = (-b + sqrt(discriminant)) / (2a);
    
    add_intersection(list, t1, obj);
    add_intersection(list, t2, obj);
}
```

### Plane Intersection
```c
// Unit plane: XZ plane at Y = 0, normal pointing +Y
void intersect_plane(t_plane *pl, t_ray ray, t_inters **list, t_object *obj)
{
    // Parallel to plane? (ray direction.y ≈ 0)
    if (fabs(ray.direction.y) < EPSILON)
        return;  // No intersection
    
    // How far along ray until y = 0?
    t = -ray.origin.y / ray.direction.y;
    
    add_intersection(list, t, obj);
}
```

### Cylinder Intersection
```c
// Unit cylinder: radius 1, along Y-axis, bounded by minimum/maximum y
void intersect_cylinder(t_cylinder *cy, t_ray ray, t_inters **list, t_object *obj)
{
    // Solve for intersection with infinite cylinder walls
    a = ray.direction.x² + ray.direction.z²;
    
    if (fabs(a) < EPSILON)
        return;  // Ray parallel to Y-axis
    
    b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    c = ray.origin.x² + ray.origin.z² - 1;  // -1 for unit radius
    
    discriminant = b² - 4ac;
    if (discriminant < 0)
        return;
    
    t0 = (-b - sqrt(disc)) / (2a);
    t1 = (-b + sqrt(disc)) / (2a);
    
    // Check if intersections are within height bounds
    for each t:
        y = ray.origin.y + t * ray.direction.y;
        if (cy->minimum < y < cy->maximum)
            add_intersection(list, t, obj);
    
    // Check caps if closed
    if (cy->closed)
        intersect_caps(cy, ray, list, obj);
}
```

### Cone Intersection
```c
// Unit cone: tip at origin, along Y-axis, angle 45°
void intersect_cone(t_cone *co, t_ray ray, t_inters **list, t_object *obj)
{
    // Transform ray by inverse of cone's transform
    local_ray = transform_ray(ray, inverse(co->trans));
    
    // For unit cone: x² + z² = y²
    a = dx² - dy² + dz²;
    b = 2*(ox*dx - oy*dy + oz*dz);
    c = ox² - oy² + oz²;
    
    // Similar solving as cylinder...
    // Check y bounds: 0 ≤ y ≤ 1 (for unit cone)
}
```

### Finding the Hit
```c
t_inters *hit(t_inters *intersections)
{
    // Return first intersection with t > 0
    while (intersections) {
        if (intersections->t > 0)
            return intersections;
        intersections = intersections->next;
    }
    return NULL;
}
```

---

## 💡 Shading & Lighting

### Phong Reflection Model

The Phong model combines three components:

```
Color = Ambient + Diffuse + Specular
```

```
           Light
             ↓
             ● ─────────→ Reflection
            /│\
           / │ \
          /  │  \
     Diffuse │   Specular
         \   │   /
          \  ↓  /
           \ ● /  ← Surface point
            \│/
             │
          Surface Normal
```

### Lighting Calculation
```c
t_tuple lighting(t_material mat, t_light light, t_tuple point, 
                 t_tuple eyev, t_tuple normalv, bool in_shadow)
{
    // Effective color (material * light)
    effective_color = hadamard_product(mat.color, light.color);
    
    // Direction to light
    lightv = normalize(light.position - point);
    
    // AMBIENT: constant background illumination
    ambient = effective_color * mat.ambient;
    
    if (in_shadow)
        return ambient;  // Shadow = ambient only
    
    // DIFFUSE: Lambert's cosine law
    light_dot_normal = dot(lightv, normalv);
    if (light_dot_normal < 0) {
        diffuse = black;
        specular = black;
    } else {
        diffuse = effective_color * mat.diffuse * light_dot_normal;
        
        // SPECULAR: Phong highlights
        reflectv = reflect(-lightv, normalv);
        reflect_dot_eye = dot(reflectv, eyev);
        
        if (reflect_dot_eye <= 0) {
            specular = black;
        } else {
            factor = pow(reflect_dot_eye, mat.shininess);
            specular = light.color * mat.specular * factor;
        }
    }
    
    return ambient + diffuse + specular;
}
```

### Material Properties
```c
typedef struct s_material {
    t_tuple color;        // Base color (R, G, B, 0)
    float   ambient;      // Ambient coefficient (0.1 default)
    float   diffuse;      // Diffuse coefficient (0.9 default)
    float   specular;     // Specular coefficient (0.9 default)
    float   shininess;    // Specular exponent (200 default)
    float   reflective;   // Mirror reflection (0.0 - 1.0)
    float   transparency; // Transparency (0.0 - 1.0)
    float   refract_index;// Refractive index (1.0 - 3.0)
    // ... texture fields
} t_material;
```

---

## 🌑 Shadows

### Shadow Ray Testing
```c
bool is_shadowed(t_world world, t_tuple point, t_light *light)
{
    // Vector from point to light
    v = light->origin - point;
    distance = magnitude(v);
    direction = normalize(v);
    
    // Cast shadow ray from point toward light
    shadow_ray = ray(point, direction);
    
    // Check for any intersection between point and light
    return intersect_world_shadow(world, shadow_ray, distance);
}
```

### Shadow Intersection Check
```c
bool intersect_world_shadow(t_world world, t_ray ray, float max_distance)
{
    for each object:
        intersect_shape(object, ray, &intersections);
        
        for each intersection:
            // Must be positive (in front of point)
            // Must be closer than the light
            if (t > EPSILON && t < max_distance)
                return true;  // In shadow
    
    return false;  // Not in shadow
}
```

### Over Point (Shadow Acne Prevention)
```c
// Offset hit point slightly along normal to prevent self-intersection
comps->over_point = add_tuple(comps->point, 
                              tuple_scalar_mult(comps->normalv, EPSILON));
```

---

## 🪞 Reflection

### Recursive Reflection
```c
t_tuple reflected_color(t_world world, t_comps *comps, int depth)
{
    if (depth <= 0)
        return black;  // Recursion limit
    
    reflective = get_material_reflective(comps->obj);
    if (reflective <= 0.0)
        return black;  // Not reflective
    
    // Create reflection ray
    reflect_ray = ray(comps->over_point, comps->reflectv);
    
    // Recursively trace reflection
    color = color_at(world, reflect_ray, depth - 1);
    
    return color * reflective;
}
```

### Reflection Vector Calculation
```c
t_tuple reflect(t_tuple in, t_tuple normal)
{
    // r = in - 2 * dot(in, n) * n
    return in - normal * 2 * dot(in, normal);
}
```

### Illustration
```
    Incident Ray        Reflected Ray
         \                  /
          \                /
           \    Normal    /
            \     ↑      /
             \    │     /
              \   │    /
               \  │   /
                \ │  /
                 \│ /
    ──────────────●──────────────  Surface
```

---

## 🔮 Refraction

### Snell's Law
```
n₁ sin(θ₁) = n₂ sin(θ₂)
```

Where:
- n₁ = refractive index of material ray is leaving
- n₂ = refractive index of material ray is entering
- θ₁ = angle of incidence
- θ₂ = angle of refraction

### Refracted Color Calculation
```c
t_tuple refracted_color(t_world world, t_comps *comps, int depth)
{
    if (depth <= 0)
        return black;
    
    transparency = get_transparency(comps->obj);
    if (transparency <= 0.0)
        return black;
    
    // Ratio of refractive indices
    n_ratio = comps->n1 / comps->n2;
    
    // Check for total internal reflection
    cos_i = dot(comps->eyev, comps->normalv);
    sin2_t = n_ratio² * (1 - cos_i²);
    
    if (sin2_t > 1.0)
        return black;  // Total internal reflection
    
    // Compute refracted ray direction
    cos_t = sqrt(1.0 - sin2_t);
    direction = normalv * (n_ratio * cos_i - cos_t) - eyev * n_ratio;
    
    refract_ray = ray(comps->under_point, direction);
    
    return color_at(world, refract_ray, depth - 1) * transparency;
}
```

### Fresnel Effect (Schlick Approximation)
Balances reflection vs refraction based on viewing angle:
```c
float schlick(t_comps *comps)
{
    cos = dot(comps->eyev, comps->normalv);
    
    // Total internal reflection check
    if (comps->n1 > comps->n2) {
        n = comps->n1 / comps->n2;
        sin2_t = n² * (1 - cos²);
        if (sin2_t > 1.0)
            return 1.0;  // Total internal reflection
        cos = sqrt(1.0 - sin2_t);
    }
    
    r0 = ((n1 - n2) / (n1 + n2))²;
    return r0 + (1 - r0) * pow(1 - cos, 5);
}
```

### Under Point (Refraction Origin)
```c
// Offset hit point slightly inside the surface
comps->under_point = subtract_tuple(comps->point,
                                    tuple_scalar_mult(comps->normalv, EPSILON));
```

---

## 🎨 Textures & UV Mapping

### UV Coordinates
Convert 3D point to 2D texture coordinates (0-1 range):

#### Sphere UV Mapping
```c
t_tuple sphere_uv_mapping(t_tuple point)
{
    // Point is in object space (unit sphere at origin)
    theta = atan2(point.x, point.z);
    phi = acos(point.y);
    
    u = 1 - (theta / (2 * PI) + 0.5);
    v = 1 - phi / PI;
    
    // Rotate texture so front faces camera
    u = u + 0.25;  // 90° rotation
    if (u > 1.0) u -= 1.0;
    
    return (t_tuple){u, v, 0, 0};
}
```

#### Plane UV Mapping
```c
t_tuple plane_uv_mapping(t_tuple point)
{
    // Simple planar mapping
    u = fmod(point.x, 1.0);
    v = fmod(point.z, 1.0);
    if (u < 0) u += 1.0;
    if (v < 0) v += 1.0;
    return (t_tuple){u, v, 0, 0};
}
```

#### Cylinder UV Mapping
```c
t_tuple cylinder_uv_mapping(t_tuple point)
{
    theta = atan2(point.x, point.z);
    u = 1 - (theta / (2 * PI) + 0.5);
    v = fmod(point.y, 1.0);
    
    // Rotate to align front
    u = u + 0.25;
    if (u > 1.0) u -= 1.0;
    
    return (t_tuple){u, v, 0, 0};
}
```

### Texture Sampling
```c
t_tuple sample_color_from_texture(mlx_texture_t *tex, float u, float v)
{
    // Convert UV to pixel coordinates
    x = (int)(u * (tex->width - 1));
    y = (int)((1 - v) * (tex->height - 1));  // Flip V
    
    // Clamp to texture bounds
    x = clamp(x, 0, tex->width - 1);
    y = clamp(y, 0, tex->height - 1);
    
    // Read RGBA from texture
    idx = (y * tex->width + x) * 4;
    r = tex->pixels[idx] / 255.0;
    g = tex->pixels[idx + 1] / 255.0;
    b = tex->pixels[idx + 2] / 255.0;
    
    return (t_tuple){r, g, b, 0};
}
```

---

## 🏔️ Bump Mapping

Bump mapping creates the illusion of surface detail by perturbing surface normals based on a height map.

### Concept

```
Without Bump Map:              With Bump Map:
                              
    Light                         Light
      ↓                            ↓
      ↓                         ↙  ↓  ↘
  ────●────                   ────●────
      │                       ↗   │   ↖
   Normal                    Perturbed
 (straight up)                normals
      
  Flat shading                Varied shading
  (boring!)                   (looks bumpy!)
```

The geometry stays flat, but the normals vary, making light reflect in different directions.

### How It Works

1. **Sample height at current UV** - Get grayscale value h(u,v)
2. **Sample height at neighboring UVs** - Get h(u+ε,v) and h(u,v+ε)
3. **Calculate gradient** - du = h(u+ε,v) - h(u,v), dv = h(u,v+ε) - h(u,v)
4. **Perturb normal** - Add tangent/bitangent vectors scaled by gradient

### Sampling Height
```c
float sample_bump_map(mlx_texture_t *texture, float u, float v)
{
    x = (uint32_t)(u * (texture->width - 1));
    y = (uint32_t)(v * (texture->height - 1));
    
    // Clamp to bounds
    if (x >= texture->width) x = texture->width - 1;
    if (y >= texture->height) y = texture->height - 1;
    
    index = (y * texture->width + x) * 4;
    r = texture->pixels[index + 0];
    g = texture->pixels[index + 1];
    b = texture->pixels[index + 2];
    
    // Convert to grayscale height [0, 1]
    height = (r + g + b) / (3.0f * 255.0f);
    return height;
}
```

### Tangent Space Basis

To perturb the normal, we need tangent and bitangent vectors:

```
          Normal (N)
              ↑
              │
              │
    Bitangent ●───────→ Tangent
       (B)              (T)
              
    T, B, N form an orthonormal basis
```

#### For Sphere:
```c
void get_tangent_basis_sphere(t_tuple point, t_tuple *tangent, t_tuple *bitangent)
{
    theta = atan2f(point.x, point.z);
    *tangent = {-sin(theta), 0, cos(theta), 0};  // Around equator
    *bitangent = cross(*tangent, point);          // Toward poles
    *tangent = normalize(*tangent);
    *bitangent = normalize(*bitangent);
}
```

#### For Plane:
```c
void get_tangent_basis_plane(t_tuple *tangent, t_tuple *bitangent)
{
    *tangent = {1, 0, 0, 0};    // Along X
    *bitangent = {0, 0, 1, 0};  // Along Z
}
```

#### For Cylinder:
```c
void get_tangent_basis_cylinder(t_tuple point, t_tuple *tangent, t_tuple *bitangent)
{
    theta = atan2f(point.x, point.z);
    *tangent = {-sin(theta), 0, cos(theta), 0};  // Around cylinder
    *bitangent = {0, 1, 0, 0};                    // Along height
}
```

### Normal Perturbation Algorithm
```c
t_tuple perturb_normal_with_bump(t_material mat, t_tuple normal,
                                  t_tuple local_point, t_obj_type type)
{
    if (!mat.has_bump_map || mat.bump_map == NULL)
        return normal;
    
    epsilon = 0.001f;
    uv = get_uv_coords(local_point, type);
    
    // Sample heights at center and neighbors
    h_center = sample_bump_map(mat.bump_map, uv.x, uv.y);
    h_u = sample_bump_map(mat.bump_map, uv.x + epsilon, uv.y);
    h_v = sample_bump_map(mat.bump_map, uv.x, uv.y + epsilon);
    
    // Calculate gradients (scaled by bump strength)
    du = (h_u - h_center) * mat.bump_strength;
    dv = (h_v - h_center) * mat.bump_strength;
    
    // Get tangent space basis
    get_tangent_basis(local_point, type, &tangent, &bitangent);
    
    // Perturb normal by gradient
    perturbed = normal + tangent * (-du) + bitangent * (-dv);
    
    return normalize(perturbed);
}
```

### Integration in Rendering Pipeline

Bump mapping is applied in `prepare_computations()` after getting the base normal:

```c
t_comps *prepare_computations(t_inters *intersection, t_ray ray, ...)
{
    // ... setup ...
    
    // Get geometric normal
    get_normal_vector(comp);
    
    // Apply bump/normal mapping (perturbs normalv)
    apply_texture_mapping(comp);
    
    // Check if inside object
    if (dot(comp->normalv, comp->eyev) < 0) {
        comp->inside = true;
        comp->normalv = negate(comp->normalv);
    }
    
    // ... rest of setup ...
}
```

### Bump Map vs Normal Map

| Aspect | Bump Map | Normal Map |
|--------|----------|------------|
| **Input** | Grayscale height map | RGB direction vectors |
| **Calculation** | Derives normals from gradient | Reads normals directly |
| **Quality** | Good for subtle detail | Better for complex detail |
| **Image appearance** | Any grayscale image | Blue-purple tint |
| **Creation** | Easy (any texture works) | Requires special tools |

### Normal Map Sampling
```c
t_tuple sample_normal_map(mlx_texture_t *texture, float u, float v)
{
    // ... get pixel ...
    
    // Convert RGB [0,255] to normal [-1,1]
    normal.x = texture->pixels[idx + 0] / 127.5f - 1.0f;
    normal.y = texture->pixels[idx + 1] / 127.5f - 1.0f;
    normal.z = texture->pixels[idx + 2] / 127.5f - 1.0f;
    
    return normalize(normal);
}
```

---

## 🔲 Patterns

Procedural patterns generate colors based on point position.

### Pattern Interface
```c
typedef struct s_pattern {
    t_tuple a, b;                    // Two pattern colors
    t_matrix transform;              // Pattern transformation
    t_matrix inv_transform;          // Inverse for efficiency
    bool has_transform;
    t_tuple (*at)(t_pattern, t_tuple);  // Function pointer to pattern function
} t_pattern;
```

### Stripe Pattern
```c
t_tuple stripe_at(t_pattern pattern, t_tuple point)
{
    if ((int)floor(point.x) % 2 == 0)
        return pattern.a;
    return pattern.b;
}
```

### Ring Pattern
```c
t_tuple ring_at(t_pattern pattern, t_tuple point)
{
    distance = sqrt(point.x² + point.z²);
    if ((int)floor(distance) % 2 == 0)
        return pattern.a;
    return pattern.b;
}
```

### Checkers Pattern
```c
t_tuple checkers_at(t_pattern pattern, t_tuple point)
{
    sum = floor(point.x) + floor(point.y) + floor(point.z);
    if ((int)sum % 2 == 0)
        return pattern.a;
    return pattern.b;
}
```

---

## 🖼️ Rendering Pipeline

### Main Render Loop
```c
void render(t_camera cam, t_world world, mlx_image_t *img)
{
    for (y = 0; y < cam.v_view; y++) {
        for (x = 0; x < cam.h_view; x++) {
            // Generate ray for this pixel
            ray = ray_for_pixel(cam, x, y);
            
            // Trace ray and get color
            color = color_at(world, ray, MAX_RECURSION_DEPTH);
            
            // Clamp color to valid range
            color = clamp_color(color, 0.0, 1.0);
            
            // Convert to 32-bit RGBA
            rgba = color_to_rgba(color);
            
            // Write to framebuffer
            mlx_put_pixel(img, x, y, rgba);
        }
    }
}
```

### Color At (Main Tracing Function)
```c
t_tuple color_at(t_world world, t_ray ray, int depth)
{
    // Find all intersections
    intersections = intersect_world(world, ray);
    
    // Get first visible hit
    h = hit(intersections);
    
    if (h == NULL) {
        free_intersections(intersections);
        return black;  // Background color
    }
    
    // Prepare hit computations
    comps = prepare_computations(h, ray, intersections);
    
    // Calculate final color
    color = shade_hit(world, &comps, depth);
    
    free_intersections(intersections);
    return color;
}
```

### Shade Hit (Final Color Assembly)
```c
t_tuple shade_hit(t_world world, t_comps *comps, int depth)
{
    // Get texture color if present
    texture_color = sample_texture_at(comps);
    
    // Start with ambient contribution
    surface = get_ambient_contribution(comps, world.ambient_color);
    
    // Add contribution from each light
    for each light in world:
        in_shadow = is_shadowed(world, comps->over_point, light);
        light_contrib = lighting(mat, light, comps, in_shadow);
        surface = surface + light_contrib;
    
    // Apply texture modulation
    surface = hadamard_product(surface, texture_color);
    
    // Handle transparency
    opaque = surface * (1 - transparency);
    
    // Recursive reflection and refraction
    reflected = reflected_color(world, comps, depth);
    refracted = refracted_color(world, comps, depth);
    
    // Fresnel blending for reflective + refractive surfaces
    if (has_both_reflection_and_refraction) {
        reflectance = schlick(comps);
        final = opaque + reflected * reflectance + refracted * (1 - reflectance);
    } else {
        final = opaque + reflected + refracted;
    }
    
    return final;
}
```

---

## 📊 Performance Considerations

### Recursion Depth
- Default: 5 levels
- Prevents infinite recursion in mirror rooms
- Each level adds reflection/refraction rays

### Epsilon Values
```c
#define EPSILON 0.0001f     // General precision
#define SHADOW_EPSILON 0.001f  // Shadow offset
```

### Optimization Techniques
1. **Inverse matrix caching**: Compute once, reuse
2. **Sorted intersections**: Early exit for shadows
3. **Linked list objects**: Easy traversal
4. **Texture caching**: Load once, sample many times

---

*miniRT Technical Architecture Guide v1.0*
