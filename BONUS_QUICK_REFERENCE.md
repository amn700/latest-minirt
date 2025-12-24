# Quick Reference - miniRT Bonus Patterns

## What Works Now ✅

### Three Pattern Types
1. **Stripe**: Alternates between two colors based on X coordinate
   ```
   pat:stripe pa:255:0:0 pb:0:0:255  # Red/Blue stripes
   ```

2. **Ring**: Alternates based on distance from XZ plane
   ```
   pat:ring pa:255:255:255 pb:0:0:0  # White/Black concentric rings
   ```

3. **Checkers**: 3D checkerboard pattern
   ```
   pat:checkers pa:255:255:255 pb:0:0:0  # White/Black checkers
   ```

### Transform Support
```
pscale:0.5:1:1      # Scale in X by 0.5
ptrans:1:2:3        # Translate by 1,2,3
prot_x:45           # Rotate 45° around X
prot_y:90           # Rotate 90° around Y
prot_z:-30          # Rotate -30° around Z
```

### Complete Example
```
sp 0,0,0  2  255,0,0  pat:stripe pa:255:255:255 pb:255:0:0 pscale:0.5:1:1
```
Creates a sphere at origin with radius 2, red base color, stripe pattern
with white and red colors, scaled to 0.5 in X direction.

## Scene Format

```
A <brightness> <R>,<G>,<B>           # Ambient light
C <x,y,z> <look_at> <fov>            # Camera
L <x,y,z> <brightness> <R>,<G>,<B>   # Light source

sp <x,y,z> <radius> <R>,<G>,<B> [pattern_params]     # Sphere
pl <x,y,z> <nx,ny,nz> <R>,<G>,<B> [pattern_params]   # Plane
cy <x,y,z> <nx,ny,nz> <radius> <height> <R>,<G>,<B> [pattern_params]  # Cylinder
```

## Light Brightness

- Range: 0.0 to 1.0
- Applied to diffuse and specular components
- Default: 1.0 if not specified

## Material Parameters (Optional)

```
ref:0.5              # Reflectivity 0-1
refr:0.8:1.5         # Transparency, refractive index
shine:200            # Shininess exponent (1-1000)
shine:0.8:100        # Specular coefficient + shininess
```

## How to Render

```bash
cd /home/amn/Desktop/miniRT/bonus
./miniRT_bonus ../maps/patterns_test.rt
# Press 'T' to save PPM
# Press 'ESC' to exit
```

## Output

- **PPM File**: output.ppm (1920x1080, ASCII P3 format)
- **Display**: Real-time MLX42 window during rendering
- **Format**: True color (8-bit per channel)

## Test Scene

Ready-to-use test scene with all features:
```
../maps/patterns_test.rt
```

Includes:
- Stripe sphere with scale
- Ring plane with rotation  
- Checkers cylinder with translation
- Single light source
- Ambient lighting

## Colors

RGB values use 0-255 range in .rt files, internally converted to 0.0-1.0
```
255,0,0      → Red (1.0, 0.0, 0.0)
0,255,0      → Green (0.0, 1.0, 0.0)
0,0,255      → Blue (0.0, 0.0, 1.0)
255,255,255  → White (1.0, 1.0, 1.0)
```

## Key Formula - Phong Shading

```
Ambient = color × ambient_coefficient × ambient_light_color
Diffuse = effective_color × diffuse_coeff × (L·N) × light_brightness
Specular = light_color × spec_coeff × pow(R·V, shininess) × light_brightness
Final = Ambient + Diffuse + Specular + Reflections + Refractions
```

Where:
- `effective_color = pattern_color ⊙ light_color` (hadamard product)
- `L` = light direction vector
- `N` = surface normal
- `R` = reflection vector
- `V` = view vector
