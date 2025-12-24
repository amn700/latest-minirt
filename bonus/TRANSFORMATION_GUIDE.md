# miniRT Transformation Pipeline Guide

## Overview

All 3D objects in miniRT use a consistent transformation pipeline to convert between **world space** and **object space**. This allows intersection calculations to happen in a normalized, simple local coordinate system.

### Pipeline: World Space → Object Space

```
World Ray
    ↓
transform_ray(ray, inverse(object.transform))
    ↓
Object Space Ray (local coordinates)
    ↓
Intersection Algorithm (simple math in object space)
    ↓
Local Intersection Point (t, y, x, z)
    ↓
Transform back to world space (multiply by inverse transpose for normals)
    ↓
World Space Normal + Shading
```

## Transform Composition

Each object's transform matrix is built as:

```
transform = Translation * Rotation * Scale
```

Applied right-to-left (matrix multiplication order):
1. **Scale** (applied first to the point)
2. **Rotation** (using quaternions via `align_y_to_vector`)
3. **Translation** (applied last)

### Example for Cone

```c
// Scene: co 0,3,0 0,1,0 10 6 0 50,50,200
// Position: (0,3,0), Axis: (0,1,0), Diameter: 10, Height: 6, Closed: 0, Color: RGB(50,50,200)

t_matrix position = translation(0, 3, 0);           // Move to world position
t_matrix align = align_y_to_vector((0,1,0));        // Rotate to match axis (identity here)
t_matrix scale = scaling(5, 6, 5);                  // radius=diameter/2, scale y by height
t_matrix transform = position * align * scale;
```

## Object-Specific Details

### Sphere
- **Local Space**: Unit sphere at origin with radius 1
- **Transform**: `translation(x, y, z) * scaling(radius, radius, radius)`
- **Bounds**: Implicitly bounded by radius
- **Intersection**: Simple quadratic equation for ray-sphere
- **Normal**: Points outward from center

```
Local space: sphere with center at (0,0,0), radius 1
After scale: radius = actual_diameter/2
After translate: center = world position
```

### Plane
- **Local Space**: Infinite plane at y=0 (XZ plane)
- **Transform**: `translation(x, y, z) * rotation_to_normal_axis`
- **Bounds**: Infinite (no bounds check)
- **Intersection**: Single ray-plane intersection (t = -origin.y / direction.y)
- **Normal**: Always (0, 1, 0) in local space, transformed to world

```
Local space: infinite XZ plane at y=0
After rotate: normal aligns with specified axis
After translate: plane positioned in world space
```

### Cylinder
- **Local Space**: Cylinder with radius 1, height from y=-0.5 to y=0.5, axis along Y
- **Transform**: `translation(x, y, z) * align_y_to_vector(axis) * scaling(radius, height/2, radius)`
- **Bounds**: -height/2 to +height/2 (centered at origin)
- **Intersection**: Quadratic for walls, check bounds, handle caps
- **Normal**: Radial for walls, (0, ±1, 0) for caps

```
Local space: unit cylinder, radius 1, centered at y=0, height spans ±0.5
After scale: radius = diameter/2, height stretches to ±height/2
After align: axis rotates to specified direction
After translate: positioned in world space
```

### Cone
- **Local Space**: Cone with tip at y=0, base at y=1, radius at y equals |y|
- **Transform**: `translation(x, y, z) * align_y_to_vector(axis) * scaling(radius, height, radius)`
- **Bounds**: 0 to 1 (in local space)
- **Intersection**: Quadratic on surface, check bounds, handle caps
- **Normal**: Gradient-based for surface, (0, ±1, 0) for caps

```
Local space: unit cone
  - Tip at y=0 (radius=0)
  - Base at y=1 (radius=1, diameter=2)
  - At any y, radius = |y|

After scale(radius, height, radius):
  - Tip still at y=0 (radius=0)
  - Base at y=height (radius=radius)
  - At any y, radius = y * radius

After align: cone rotates to match axis direction
After translate: tip positioned at world center
```

## Intersection Algorithm Flow

### For Any Object:

```c
// 1. Transform ray to object space
t_ray local_ray = transform_ray(world_ray, inverse(object.transform));

// 2. Find intersections in local space (simple math)
intersect_cone(&cone, local_ray, &intersections, object);

// 3. For each intersection, compute world point
intersection.t;  // parameter along ray (same in both spaces)
world_point = position(world_ray, intersection.t);

// 4. Compute normal in object space
local_point = multiply_matrix_by_tuple(inverse(transform), world_point);
local_normal = compute_normal(shape, local_point);

// 5. Transform normal to world space
// Use inverse transpose of transform for normal transformation
world_normal = multiply_matrix_by_tuple(transpose(inverse(transform)), local_normal);
```

## Key Principles

### 1. **Ray Parameter Stays Same**
The `t` value (distance along ray) is identical in both world and object space because:
```
local_ray.origin + t * local_ray.direction
= inverse(M) * world_origin + t * inverse(M) * world_direction
= inverse(M) * (world_origin + t * world_direction)
```

### 2. **Bounds Checking in Local Space**
Always check intersection bounds using the **local space coordinates** because:
- Local space is normalized (sphere radius=1, cone y∈[0,1], etc.)
- Bounds are defined in local space
- After scaling, world dimensions are hidden in the transform matrix

### 3. **Normals Use Inverse Transpose**
For non-uniform scaling or shearing, normals must be transformed by `transpose(inverse(M))` not just the inverse, to remain perpendicular to surfaces.

### 4. **Quaternion Rotation**
The `align_y_to_vector` function uses quaternions internally:
```
q = quaternion from axis angle
rotation_matrix = quat_to_matrix(q)
```
This avoids gimbal lock and is numerically stable for arbitrary axis rotations.

## Debugging Transformations

### Check if cone appears too large/small:
- Verify `radius = diameter / 2`
- Verify `scale(radius, height, radius)` has correct radius and height
- Check scene format: `co x y z ax ay az diameter height closed R G B`

### Check if cone points wrong direction:
- Verify axis is normalized: `sqrt(ax² + ay² + az²) ≈ 1`
- Verify `align_y_to_vector(axis)` with special cases:
  - `axis = (0,1,0)` → identity matrix (cone points up)
  - `axis = (0,-1,0)` → flip Y (cone points down)
  - Any other → quaternion-based rotation

### Check if shadows are wrong:
- Ensure bounds are correct in local space
- For cone: bounds are `[0, 1]` (not `[0, height]`)
- Shadow ray test uses `t > EPSILON && t < max_distance`

### Check if intersections are missing:
- Ray must be transformed to local space FIRST
- Bounds check uses local y coordinate
- Verify strict vs. inclusive comparisons: `>` vs `>=`

## Examples

### Rendering a cone at (0, 3, 0) pointing up with height 6:

**Scene file:**
```
co 0 3 0   0 1 0   10 6 0   50 50 200
```

**What happens:**
1. Cone created with axis=(0,1,0), height=6, diameter=10
2. Transform = T(0,3,0) × Align(0,1,0) × Scale(5,6,5)
   - Align gives identity (already pointing up)
   - Result: translate to y=3, scale by radius=5 and height=6
3. Ray hits at world point, transformed to local space
4. Check: is local_y ∈ [0, 1]? (bounds check)
5. Compute normal using gradient formula
6. Transform normal to world space using transpose of inverse

### Rendering a cone at (0, 3, 0) pointing sideways (0, 0, 1):

**Scene file:**
```
co 0 3 0   0 0 1   10 6 0   50 50 200
```

**What happens:**
1. Cone created with axis=(0,0,1), height=6, diameter=10
2. Transform = T(0,3,0) × Align(0,0,1) × Scale(5,6,5)
   - Align rotates from Y-axis to Z-axis using quaternion
   - Rotation matrix applied
3. Same intersection logic as before, but now in rotated local space
4. Normal transformation uses transpose of inverse to account for rotation

## Files Involved

- `src/geometry/cone_base_bonus.c`: Cone creation and normal computation
- `src/geometry/cone_intersect_bonus.c`: Cone-ray intersection algorithm
- `src/parsing/cone_extract_bonus.c`: Parse scene file and build transform
- `src/math/axis_align_bonus.c`: Quaternion-based rotation matrix (`align_y_to_vector`)
- `src/core/intersections_bonus.c`: Ray transformation and object dispatch
- `src/core/computations_bonus.c`: Normal transformation to world space

## References

- **The Ray Tracer Challenge** by Jamis Buck - Ray-object intersection and transformation
- **m4nnb3ll/miniRT** - Reference implementation of quaternion-based rotations
