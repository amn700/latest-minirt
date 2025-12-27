# Texture Mapping Feature - miniRT

## Overview

This implementation adds comprehensive texture support to the miniRT ray tracer, including:
- **Bump Mapping**: Height-based surface detail using grayscale textures
- **Normal Mapping**: RGB normal maps for accurate surface detail
- Full parsing integration with the scene file format
- Proper error handling and validation

## Scene File Format

### Bump Mapping

Add bump maps to objects using the `bump:` and `bump_strength:` parameters:

```
sp 0,1,0 2.0 200,200,200 bump:z_textures/red_mini.png bump_strength:0.7
pl 0,0,0 0,1,0 128,128,128 bump:textures/wood_bump.png bump_strength:0.5
cy 0,0,0 0,1,0 1.0 2.0 150,150,150 bump:textures/brick_bump.png
```

**Parameters:**
- `bump:path/to/texture.png` - Path to grayscale bump map texture
- `bump_strength:0.0-1.0` - Strength of bump effect (default: 0.5)

### Normal Mapping

Add normal maps using the `normal:` and `normal_strength:` parameters:

```
sp 0,1,0 2.0 200,200,200 normal:textures/stone_normal.png normal_strength:1.0
```

**Parameters:**
- `normal:path/to/texture.png` - Path to RGB normal map texture
- `normal_strength:0.0-1.0` - Blending strength (default: 0.5)

**Note:** If both bump and normal maps are specified, the normal map takes priority.

## Texture File Requirements

- **Format**: PNG files (loaded via `mlx_load_png()`)
- **Bump Maps**: Grayscale images where brighter = higher
- **Normal Maps**: RGB images where:
  - Red channel = X component of normal
  - Green channel = Y component of normal
  - Blue channel = Z component of normal
- **Path**: Relative or absolute paths from the executable
- **Performance**: Textures larger than 4096x4096 may impact performance

## UV Mapping

Each object type has specific UV coordinate mapping:

### Sphere
- U: Longitude mapping using atan2(x, z)
- V: Latitude mapping using asin(y)
- Range: [0, 1] × [0, 1]

### Plane
- U: X coordinate with tiling
- V: Z coordinate with tiling
- Automatic wrapping for repeating textures

### Cylinder
- U: Circumference using atan2(x, z)
- V: Height (Y) with tiling

### Cone
- U: Circumference using atan2(x, z)
- V: Height (Y) with tiling

## Error Handling

The texture system includes comprehensive error handling:

- ✅ File existence validation before loading
- ✅ Clear error messages for missing files
- ✅ Graceful degradation (renders without texture on failure)
- ✅ Texture dimension warnings (> 4096x4096)
- ✅ Strength parameter validation (must be 0.0-1.0)
- ✅ Duplicate parameter detection

**Example error messages:**
```
Error: Failed to load bump map 'path/to/missing.png'
Error: Invalid bump strength in 'bump_strength:1.5'
Warning: Texture 'large.png' is large (8192x8192), may impact performance
```

## Example Scenes

### Basic Bump Map
```
A 0.2 255,255,255
C 0,5,-15 0,0,1 70
L -10,10,-10 0.7 255,255,255

sp 0,1,0 2.0 200,200,200 bump:z_textures/red_mini.png bump_strength:0.7
pl 0,0,0 0,1,0 128,128,128
```

### Multiple Textured Objects
```
A 0.2 255,255,255
C 0,5,-15 0,0,1 70
L -10,10,-10 0.7 255,255,255

# Sphere with bump map
sp -2,1,0 2.0 200,200,200 bump:textures/bump1.png bump_strength:0.8

# Cylinder with normal map
cy 2,0,0 0,1,0 1.0 3.0 150,150,150 normal:textures/normal1.png normal_strength:1.0

# Plain sphere (backward compatibility)
sp 0,3,0 1.5 100,150,200
```

## Memory Management

Texture resources are automatically cleaned up:
- Textures are freed when objects are destroyed
- Uses `mlx_delete_texture()` for proper cleanup
- No memory leaks with proper usage

## Backward Compatibility

All existing scene files continue to work:
- Objects without texture parameters render normally
- No changes required to existing scenes
- Texture parameters are optional

## Implementation Details

### Files Created
- `bonus/includes/texture_bonus.h` - Texture function declarations
- `bonus/src/geometry/uv_mapping_bonus.c` - UV coordinate mapping
- `bonus/src/lighting/bump_mapping_bonus.c` - Bump/normal map application
- `bonus/src/parsing/texture_parsing_bonus.c` - Texture loading and parsing

### Files Modified
- `bonus/includes/types_bonus.h` - Extended t_material structure
- `bonus/includes/geometry_bonus.h` - Added UV mapping declarations
- `bonus/includes/miniRT_bonus.h` - Included texture header
- `bonus/src/core/computations_bonus.c` - Integrated texture mapping
- `bonus/src/parsing/material_params_bonus.c` - Extended parameter parsing
- `bonus/src/main_bonus.c` - Added texture cleanup
- `bonus/Makefile` - Added new source files

## Testing

To test the texture features:

1. **Basic functionality:**
   ```bash
   ./miniRT_bonus maps/texture_test.rt
   ```

2. **Missing texture handling:**
   Create a scene with non-existent texture path to verify error handling

3. **Backward compatibility:**
   Run existing scene files to ensure they still work

4. **Memory leaks:**
   Run with valgrind to verify no memory leaks:
   ```bash
   valgrind --leak-check=full ./miniRT_bonus maps/texture_test.rt
   ```

## Known Limitations

- Requires MLX42 library to be built and available
- Texture loading uses PNG format only (no JPEG support via mlx_load_png)
- Performance scales with texture size and complexity
- Tangent space calculation is simplified for planes

## Future Enhancements

Potential improvements:
- Bilinear texture filtering for smoother results
- Texture caching to avoid loading same texture multiple times
- Support for additional texture formats
- Mipmapping for better performance with large textures
- Anisotropic filtering for improved quality at oblique angles
