# miniRT File Format Reference

Complete specification for `.rt` scene file format.

## Basic Scene Elements

### Ambient Lighting (A)
```
A <ratio> <R>,<G>,<B>
```
- **ratio**: Ambient lighting intensity (range: [0.0, 1.0])
- **R,G,B**: RGB color values (range: [0, 255] for each component)

**Example:**
```
A 0.2 255,255,255
```

### Camera (C)
```
C <x>,<y>,<z> <axis_x>,<axis_y>,<axis_z> <FOV>
```
- **x,y,z**: Camera position coordinates (any real number)
- **axis_x,axis_y,axis_z**: Normalized orientation vector (range: [-1.0, 1.0] for each component, magnitude = 1.0)
- **FOV**: Field of view in degrees (range: [0, 180])

**Example:**
```
C 0,0,-20 0,0,1 70
```

### Light (L)
```
L <x>,<y>,<z> <brightness> <R>,<G>,<B>
```
- **x,y,z**: Light position coordinates (any real number)
- **brightness**: Light intensity (range: [0.0, 1.0])
- **R,G,B**: RGB color values (range: [0, 255] for each component)

**Example:**
```
L -40,50,-30 0.7 255,255,255
```

## Geometric Objects

### Sphere (sp)
```
sp <x>,<y>,<z> <diameter> <R>,<G>,<B> [material_params]
```
- **x,y,z**: Center coordinates (any real number)
- **diameter**: Sphere diameter (range: > 0)
- **R,G,B**: RGB color values (range: [0, 255] for each component)
- **material_params**: Optional material properties (see below)

**Example:**
```
sp 0,0,10 5 255,0,0 ref:0.3 shine:100
```

### Plane (pl)
```
pl <x>,<y>,<z> <axis_x>,<axis_y>,<axis_z> <R>,<G>,<B> [material_params]
```
- **x,y,z**: Point on the plane (any real number)
- **axis_x,axis_y,axis_z**: Normalized normal vector (range: [-1.0, 1.0] for each component, magnitude = 1.0)
- **R,G,B**: RGB color values (range: [0, 255] for each component)
- **material_params**: Optional material properties (see below)

**Example:**
```
pl 0,-5,0 0,1,0 100,100,100 pat:checkers pa:255:255:255 pb:0:0:0 pscale:1:1:1
```

### Cylinder (cy)
```
cy <x>,<y>,<z> <axis_x>,<axis_y>,<axis_z> <diameter> <height> <closed> <R>,<G>,<B> [material_params]
```
- **x,y,z**: Center coordinates (any real number)
- **axis_x,axis_y,axis_z**: Normalized axis vector (range: [-1.0, 1.0] for each component, magnitude = 1.0)
- **diameter**: Cylinder diameter (range: > 0)
- **height**: Cylinder height (range: > 0)
- **closed**: Cap flag (values: 0=open, 1=closed with caps)
- **R,G,B**: RGB color values (range: [0, 255] for each component)
- **material_params**: Optional material properties (see below)

**Example:**
```
cy 5,0,10 0,1,0 3 8 1 0,255,0 ref:0.2 shine:50
```

**Note:** For backward compatibility, the `closed` field is optional (defaults to 0 if omitted).

### Cone (co)
```
co <x>,<y>,<z> <axis_x>,<axis_y>,<axis_z> <diameter> <height> <closed> <R>,<G>,<B> [material_params]
```
- **x,y,z**: Center coordinates - base center (any real number)
- **axis_x,axis_y,axis_z**: Normalized axis vector pointing from base to apex (range: [-1.0, 1.0] for each component, magnitude = 1.0)
- **diameter**: Base diameter (range: > 0)
- **height**: Cone height from base to apex (range: > 0)
- **closed**: Cap flag (values: 0=open base, 1=closed with base cap)
- **R,G,B**: RGB color values (range: [0, 255] for each component)
- **material_params**: Optional material properties (see below)

**Example:**
```
co 0,0,10 0,1,0 4 6 1 255,100,0 pat:stripe pa:255:100:0 pb:100:50:0 shine:100
```

**Note:** For backward compatibility, the `closed` field is optional (defaults to 0 if omitted).

## Material Parameters

All geometric objects support optional material parameters that can be appended in any order.

### Reflection
```
ref:<value>
```
- **value**: Reflectivity coefficient (range: [0.0, 1.0])
  - **0.0**: No reflection (matte)
  - **1.0**: Perfect mirror

**Example:** `ref:0.5`

### Shininess (Specular Highlight)
```
shine:<value>
```
- **value**: Shininess exponent (range: [10, 300])
  - **Low values (10-50)**: Broad, dull highlights
  - **Medium values (50-150)**: Standard shiny surfaces
  - **High values (150-300)**: Sharp, glossy highlights

**Example:** `shine:200`

### Refraction (Transparency)
```
refr:<transparency>:<refractive_index>
```
- **transparency**: Transparency coefficient (range: [0.0, 1.0])
  - **0.0**: Opaque
  - **1.0**: Fully transparent
- **refractive_index**: Material refractive index (range: typically [1.0, 2.5])
  - Air: 1.0
  - Water: 1.333
  - Glass: 1.5-1.9
  - Diamond: 2.417

**Example:** `refr:0.9:1.5`

### Patterns

#### Pattern Type
```
pat:<type>
```
- **type**: Pattern name
  - `stripe`: Alternating vertical stripes
  - `checkers`: 3D checkerboard pattern
  - `ring`: Concentric rings (from origin)

**Example:** `pat:checkers`

#### Pattern Primary Color
```
pa:<R>:<G>:<B>
```
- **R,G,B**: RGB values for primary pattern color (range: [0, 255] for each component)

**Example:** `pa:255:255:255`

#### Pattern Secondary Color
```
pb:<R>:<G>:<B>
```
- **R,G,B**: RGB values for secondary pattern color (range: [0, 255] for each component)

**Example:** `pb:0:0:0`

#### Pattern Scale
```
pscale:<x>:<y>:<z>
```
- **x,y,z**: Scale factors for pattern in each dimension (range: > 0, typically [0.1, 10.0])
  - Values > 1.0: Enlarge the pattern
  - Values = 1.0: Original pattern size
  - Values < 1.0: Shrink the pattern

**Example:** `pscale:0.5:0.5:0.5`

#### Pattern Translation
```
ptrans:<x>:<y>:<z>
```
- **x,y,z**: Translation offset for pattern (any real number)
  - Shifts the pattern in space

**Example:** `ptrans:0:2:0`

#### Pattern Rotation
```
prot_x:<degrees>
prot_y:<degrees>
prot_z:<degrees>
```
- **degrees**: Rotation angle in degrees (range: [0, 360] or any real number)
  - Rotates pattern around X, Y, or Z axis respectively

**Example:** `prot_y:45`

### Complete Material Example
```
sp 0,0,10 5 255,0,0 ref:0.3 shine:150 refr:0.5:1.5 pat:stripe pa:255:0:0 pb:100:0:0 pscale:0.3:0.3:0.3 prot_y:45
```

## Format Rules

1. **Comments**: Lines starting with `#` are ignored
2. **Whitespace**: Extra whitespace is ignored
3. **Order**: Scene elements can appear in any order
4. **Uniqueness**:
   - Ambient (A): Exactly one required
   - Camera (C): Exactly one required
   - Light (L): At least one required, multiple allowed
   - Objects: Unlimited number of any object type
5. **Material Parameters**: Can appear in any order after base object definition
6. **Vector Normalization**: Axis and normal vectors must be normalized
7. **Color Range**: RGB values must be [0-255]
8. **Coordinate System**: Right-handed coordinate system (Y-up)

## Complete Scene Example

```
# Scene Configuration
A 0.2 255,255,255
C 0,5,-20 0,0,1 70
L -10,20,-10 0.8 255,255,255

# Floor with checkerboard pattern
pl 0,-5,0 0,1,0 100,100,100 pat:checkers pa:255:255:255 pb:50:50:50 pscale:1:1:1 ref:0.3

# Reflective sphere
sp -5,0,10 5 255,0,0 ref:0.5 shine:200

# Transparent glass sphere
sp 5,0,10 5 200,200,255 refr:0.9:1.5 shine:300

# Striped cone
co 0,0,15 0,1,0.2 4 8 1 255,150,0 pat:stripe pa:255:150:0 pb:150:75:0 pscale:0.2:0.2:0.2 shine:100 ref:0.3

# Closed cylinder with rings
cy -8,0,8 0.2,1,0 3 6 1 0,255,100 pat:ring pa:0:255:100 pb:0:100:50 pscale:0.3:0.3:0.3 ref:0.2
```

## Notes

- **Coordinate Space**: Patterns are evaluated in object space (automatically transformed for cylinders and cones)
- **Performance**: High reflection/refraction depths may impact rendering speed
- **Caps**: Closed cylinders and cones render with flat caps at the ends
- **Apex**: Cone apex is at the tip (narrow end), base is at the wide end
- **Height**: For cylinders and cones, height is measured along the axis direction
