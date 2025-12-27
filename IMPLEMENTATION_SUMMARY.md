# Texture Mapping Implementation Summary

## Overview
This implementation adds comprehensive texture mapping and bump/normal mapping support to the miniRT ray tracer, using libpng for texture loading instead of MLX42's built-in texture functions.

## Key Features

### 1. PNG-based Texture Loading
- Custom PNG loader using libpng (not MLX42)
- Converts PNG images to internal `t_tex` structure
- Supports RGB and RGBA PNG formats
- Automatic conversion to normalized color values

### 2. UV Mapping
- **Sphere**: Spherical coordinate mapping
  - U: longitude using `atan2(x, z)`
  - V: latitude using `acos(y)`
  - Range: [0, 1] × [0, 1]

- **Plane**: Planar mapping with automatic wrapping
  - U: `x - floor(x)`
  - V: `z - floor(z)`
  - Enables seamless tiling

- **Cylinder**: Cylindrical mapping
  - U: circumference using `atan2(x, z)`
  - V: height (y-coordinate with wrapping)

### 3. Texture Sampling
- Nearest-neighbor texture sampling
- UV clamping to [0, 1] range
- Pixel coordinate calculation from UV
- Color normalization (0-255 to 0-1)

### 4. Bump/Normal Mapping
- TBN (Tangent-Bitangent-Normal) matrix construction
- Normal map sampling and remapping (RGB [0,255] → Normal [-1,1])
- Tangent space to world space transformation
- Automatic tangent/bitangent calculation based on surface normal

### 5. Parser Integration
- Extended scene file format with `tex=` and `btex=` parameters
- Backward compatible (optional texture parameters)
- File validation (checks if texture files exist)
- Support for relative and absolute paths

## File Structure

### New Files Created

**Headers:**
```
new/includes/ft_png.h           - PNG loading and texture management API
```

**PNG Loading (png/):**
```
new/png/ft_png_read.c           - PNG file reading with libpng
new/png/ft_png_img_to_tex.c     - PNG to t_tex conversion
new/png/ft_png_utils.c          - PNG writing utilities
```

**Geometry:**
```
new/src/geometry/uv_mapping.c   - UV coordinate calculation for primitives
```

**Lighting:**
```
new/src/lighting/texture_helpers.c   - Texture sampling and color lookup
new/src/lighting/normals_helpers.c   - Normal perturbation with TBN
```

**Parsing:**
```
new/src/parsing/texture_parsing.c    - Texture parameter parsing
```

### Modified Files

**Data Structures (types.h):**
- Added `t_color` structure for RGBA pixels
- Added `t_tex` structure for texture data
- Extended `t_material` with texture fields
- Extended `t_comps` with UV coordinates

**Core Files:**
- `computations.c`: Added UV calculation and normal perturbation
- `shading.c`: Integrated texture color sampling
- `sphere_extract.c`, `plane_extract.c`, `cylinder_extract.c`: Added texture parsing
- `sphere.c`: Updated material initialization

**Build System:**
- Updated Makefile to link libpng
- Added PNG source files to build
- Added pkg-config integration for libpng flags

## Data Structures

### t_tex (Texture)
```c
typedef struct s_tex
{
    int      width;    // Texture width in pixels
    int      height;   // Texture height in pixels
    t_color  **pixels; // 2D array of RGBA pixels
}   t_tex;
```

### t_material (Extended)
```c
typedef struct s_material
{
    t_tuple  color;        // Base material color
    float    ambient;      // Ambient coefficient
    float    diffuse;      // Diffuse coefficient
    float    specular;     // Specular coefficient
    float    shininess;    // Shininess factor
    t_tex    tex;          // Color texture
    t_tex    btex;         // Bump/normal texture
    bool     has_tex;      // Has color texture flag
    bool     has_btex;     // Has bump texture flag
}   t_material;
```

### t_comps (Extended)
```c
typedef struct s_computations
{
    float     t;           // Ray parameter
    t_object  *obj;        // Hit object
    t_tuple   point;       // Hit point
    t_tuple   eyev;        // Eye vector
    t_tuple   normalv;     // Surface normal
    t_tuple   over_point;  // Point slightly above surface
    bool      inside;      // Inside object flag
    float     u;           // U texture coordinate
    float     v;           // V texture coordinate
}   t_comps;
```

## Scene File Syntax

### Basic Format
```
sp x,y,z diameter r,g,b [tex=path] [btex=path]
pl x,y,z nx,ny,nz r,g,b [tex=path] [btex=path]
cy x,y,z ax,ay,az diameter height r,g,b [tex=path] [btex=path]
```

### Examples
```
# Textured sphere
sp 0,1,0 2.0 200,200,200 tex=assets/textures/checker.png

# Sphere with bump map
sp 0,1,0 2.0 200,200,200 btex=assets/textures/normal_sphere.png

# Textured and bumped
sp 0,1,0 2.0 200,200,200 tex=earth.png btex=earth_normal.png

# Textured plane
pl 0,0,0 0,1,0 128,128,128 tex=assets/textures/checker.png

# Textured cylinder
cy 0,0,0 0,1,0 1.0 3.0 150,150,150 tex=wood.png
```

## Build Requirements

### Dependencies
- libpng development libraries
- MLX42 (for rendering)
- libglfw3-dev (for MLX42)
- Standard math library

### Installation
**Ubuntu/Debian:**
```bash
sudo apt-get install libpng-dev libglfw3-dev
```

**macOS:**
```bash
brew install libpng glfw
```

### Building
```bash
cd new/
make
```

## Memory Management

### Texture Lifecycle
1. **Loading**: Textures loaded during scene parsing
2. **Storage**: Stored in material structure
3. **Cleanup**: `free_material_textures()` frees texture memory

### Memory Safety
- Proper null checking throughout
- No double-free issues
- Clean separation of PNG and texture structures
- Automatic cleanup on program exit

## Testing

### Test Scenes
Three test scenes provided in `scenes/`:
1. `textured_sphere.rt` - Basic textured sphere
2. `textured_and_bumped.rt` - Multiple objects with textures and bump maps
3. `textured_plane_cylinder.rt` - Textured plane and cylinder

### Sample Textures
Located in `assets/textures/`:
- `checker.png` - Simple checker pattern (good for testing UV mapping)
- `normal_sphere.png` - Normal map for spheres

## Rendering Pipeline

### Texture Mapping Flow
1. Ray intersection computes hit point
2. `prepare_computations()` calculates UV coordinates in object space
3. `ft_tex_color_at()` samples texture at UV coordinates
4. Texture color replaces material base color in lighting calculation

### Bump Mapping Flow
1. Ray intersection computes hit point and normal
2. UV coordinates calculated in object space
3. `perturb_normal_bump()` samples normal map at UV
4. Normal map value converted to tangent space normal
5. TBN matrix transforms to world space
6. Perturbed normal used for lighting calculations

## Implementation Notes

### Design Decisions

1. **libpng over MLX42**: 
   - Provides more control over texture loading
   - Better error handling
   - Portable across platforms
   - Industry-standard PNG library

2. **Nearest-neighbor sampling**:
   - Simple and fast
   - Suitable for ray tracing
   - Can be upgraded to bilinear if needed

3. **Automatic UV calculation**:
   - No manual UV specification needed
   - Consistent mapping per primitive type
   - Wrapping for seamless tiling

4. **TBN approach for normals**:
   - Standard computer graphics technique
   - Proper normal orientation
   - Compatible with standard normal maps

### Performance Considerations

- Texture loading done once at parse time
- UV calculation minimal overhead
- Texture sampling is O(1) lookup
- No texture filtering overhead
- Large textures consume more memory

### Future Enhancements

Potential improvements:
- Bilinear texture filtering
- Texture caching for duplicate paths
- Mipmapping for better quality at distance
- Procedural textures (noise, fractals)
- Texture transformation (scale, rotate)
- Anisotropic filtering
- Support for additional image formats (JPEG, BMP)

## Backward Compatibility

- All existing scene files work without modification
- Texture parameters are completely optional
- Default behavior unchanged when no textures specified
- No performance impact for untextured scenes

## Error Handling

### Robust Error Checking
- File existence validation before loading
- PNG format validation
- Memory allocation checks
- Graceful degradation on texture load failure
- Clear error messages

### Error Messages
- "Error: Cannot open texture file 'path'"
- "Error: Failed to load texture from 'path'"
- "Error: Empty texture path in 'tex=' parameter"

## Code Quality

### Standards Compliance
- Follows 42 norminette standards
- Consistent naming conventions
- Proper header guards
- Function length limits respected
- Comprehensive error handling

### Memory Safety
- No memory leaks
- Proper resource cleanup
- Null pointer checks
- Safe string operations

## Conclusion

This implementation provides a complete, production-ready texture mapping system for miniRT that:
- Uses industry-standard libpng
- Integrates seamlessly with existing code
- Maintains backward compatibility
- Provides clear documentation
- Includes test assets and examples
- Follows best practices for memory management and error handling

The implementation is modular, extensible, and ready for future enhancements while maintaining simplicity and clarity in the codebase.
