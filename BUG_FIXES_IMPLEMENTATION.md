# MiniRT Bonus Pattern System - Bug Fixes Implementation Guide

## Priority 1: CRITICAL FIXES (Do First)

### Fix #1: Initialize pat_color in phong_lighting_bonus.c
**File:** `bonus/src/lighting/phong_lighting_bonus.c`  
**Lines:** 47-51  
**Current Code:**
```c
if (material.pattern.at)
{
    t_tuple pat_color;
    t_tuple pos = position;
    if (material.pattern.has_transform)
        pos = multiply_matrix_by_tuple(material.pattern.inv_transform, position);
    pat_color = material.pattern.at(material.pattern, pos);
    effective_color = hadamard_product(pat_color, light.color);
}
```

**Fixed Code:**
```c
if (material.pattern.at)
{
    t_tuple pat_color = {0, 0, 0, 0};  // Initialize to prevent garbage values
    t_tuple pos = position;
    if (material.pattern.has_transform)
        pos = multiply_matrix_by_tuple(material.pattern.inv_transform, position);
    pat_color = material.pattern.at(material.pattern, pos);
    effective_color = hadamard_product(pat_color, light.color);
}
```

**Risk Mitigated:** Memory safety violation, undefined behavior

---

### Fix #2: Validate and Clamp Light Brightness
**File:** `bonus/src/lighting/phong_lighting_bonus.c`  
**Lines:** Top of file + usage in compute_lighting_components

**Add Helper Function:**
```c
static float safe_brightness(float brightness)
{
    // Clamp brightness to valid range [0.0, 1.0]
    if (brightness < 0.0f)
        return 0.0f;
    if (brightness > 1.0f)
        return 1.0f;
    return brightness;
}
```

**Replace Line 58:**
```c
// OLD:
result.diffuse = tuple_scalar_mult(tuple_scalar_mult(effective_color, \
    material.diffuse), ldn * light.brightness);

// NEW:
result.diffuse = tuple_scalar_mult(tuple_scalar_mult(effective_color, \
    material.diffuse), ldn * safe_brightness(light.brightness));
```

**Replace Line 66:**
```c
// OLD:
result.specular = tuple_scalar_mult(tuple_scalar_mult(
    tuple_scalar_mult(light.color, light.brightness), \
    material.specular), factor);

// NEW:
float brightness = safe_brightness(light.brightness);
result.specular = tuple_scalar_mult(tuple_scalar_mult(
    tuple_scalar_mult(light.color, brightness), \
    material.specular), factor);
```

**Risk Mitigated:** Rendering correctness, invalid lighting values

---

### Fix #3: Add Input Validation to new_light()
**File:** `bonus/src/core/world_bonus.c`  
**Lines:** 49-56

**Add Validation Helper at top of file:**
```c
#include <math.h>

static bool validate_color_tuple(t_tuple color)
{
    // Check for NaN or Infinity
    if (isnan(color.x) || isnan(color.y) || isnan(color.z))
        return (false);
    if (isinf(color.x) || isinf(color.y) || isinf(color.z))
        return (false);
    // Check range [0.0, 1.0]
    if (color.x < 0.0f || color.x > 1.0f)
        return (false);
    if (color.y < 0.0f || color.y > 1.0f)
        return (false);
    if (color.z < 0.0f || color.z > 1.0f)
        return (false);
    return (true);
}
```

**Replace new_light() function:**
```c
t_light *new_light(t_tuple point, t_tuple color)
{
    t_light *light;

    // Validate color input
    if (!validate_color_tuple(color))
        return (NULL);

    light = malloc(sizeof(t_light));
    if (!light)
        return (NULL);
    light->origin = point;
    light->color = color;
    light->brightness = 1.0f;
    light->next = NULL;
    return (light);
}
```

**Risk Mitigated:** Garbage value propagation, invalid lighting inputs

---

### Fix #4: Ensure W-Component in Pattern Colors
**File:** `bonus/src/paterns/patern.c`  
**Lines:** 71-95 (stripe_at, ring_at, checkers_at)

**Updated Functions:**
```c
t_tuple stripe_at(t_pattern pattern, t_tuple point)
{
    t_tuple result;
    
    if (((int)point.x) % 2 == 0)
        result = pattern.a;
    else
        result = pattern.b;
    result.w = 0;  // Ensure color tuple
    return (result);
}

t_tuple ring_at(t_pattern pattern, t_tuple point)
{
    float r;
    t_tuple result;

    r = sqrtf(point.x * point.x + point.z * point.z);
    if (((int)r) % 2 == 0)
        result = pattern.a;
    else
        result = pattern.b;
    result.w = 0;  // Ensure color tuple
    return (result);
}

t_tuple checkers_at(t_pattern pattern, t_tuple point)
{
    int sum;
    t_tuple result;

    sum = (int)point.x + (int)point.y + (int)point.z;
    if (sum % 2 == 0)
        result = pattern.a;
    else
        result = pattern.b;
    result.w = 0;  // Ensure color tuple
    return (result);
}
```

**Risk Mitigated:** Color tuple correctness, improper w-component values

---

## Priority 2: CONSISTENCY FIXES (Do Second)

### Fix #5: Harmonize Light Brightness Application
**File:** `bonus/src/lighting/phong_lighting_bonus.c`  
**Lines:** 54-68

**Current Issue:** Brightness is applied inconsistently to diffuse vs specular components.

**Unified Approach (Option A - Recommended):**
```c
static t_lighting_components compute_lighting_components(t_material material, \
    t_light light, t_tuple position, t_tuple eyev, t_tuple normalv, \
    bool in_shadow, t_tuple ambient_color)
{
    t_lighting_components result;
    t_tuple effective_color;
    t_tuple lightv;
    float ldn;
    t_tuple reflectv;
    float rde;
    float factor;
    float brightness = safe_brightness(light.brightness);  // Use consistent brightness

    // ... ambient code ...

    result.diffuse = (t_tuple){0, 0, 0, 0};
    result.specular = (t_tuple){0, 0, 0, 0};
    if (in_shadow)
        return (result);
    
    if (material.pattern.at)
    {
        t_tuple pat_color = {0, 0, 0, 0};
        t_tuple pos = position;
        if (material.pattern.has_transform)
            pos = multiply_matrix_by_tuple(material.pattern.inv_transform, position);
        pat_color = material.pattern.at(material.pattern, pos);
        effective_color = hadamard_product(pat_color, light.color);
    }
    else
        effective_color = hadamard_product(material.color, light.color);
    
    lightv = normalizing_vector(substract_tuple(light.origin, position));
    ldn = vecs_dot_product(lightv, normalv);
    
    if (ldn >= 0)
    {
        // Apply brightness uniformly
        result.diffuse = tuple_scalar_mult(
            tuple_scalar_mult(effective_color, material.diffuse),
            ldn * brightness);
    }
    
    reflectv = normalizing_vector(substract_tuple(
        tuple_scalar_mult(normalv, 2.0 * ldn), lightv));
    rde = vecs_dot_product(reflectv, eyev);
    
    if (rde > 0)
    {
        factor = pow(rde, material.shininess);
        // Apply brightness uniformly
        result.specular = tuple_scalar_mult(
            tuple_scalar_mult(
                tuple_scalar_mult(light.color, material.specular),
                factor),
            brightness);
    }
    return (result);
}
```

**Risk Mitigated:** Mathematical consistency, symmetric lighting calculations

---

### Fix #6: Extract Pattern Color Helper Function
**File:** `bonus/src/paterns/patern.c`  
**Add to file:**

```c
/**
 * @brief Sample pattern at given point with transform handling
 * @param pattern Pattern to sample
 * @param point Point in world/object space
 * @return Color tuple at point (w=0)
 */
static t_tuple pattern_color_at(t_pattern pattern, t_tuple point)
{
    t_tuple pos = point;
    t_tuple result;

    if (pattern.has_transform)
        pos = multiply_matrix_by_tuple(pattern.inv_transform, point);
    
    if (pattern.at == NULL)
        return ((t_tuple){0, 0, 0, 0});  // Fallback
    
    result = pattern.at(pattern, pos);
    result.w = 0;  // Ensure color tuple
    return (result);
}
```

**Update phong_lighting_bonus.c to use this function:**
```c
// Replace lines 47-51 with:
if (material.pattern.at)
{
    t_tuple pat_color = pattern_color_at(material.pattern, position);
    effective_color = hadamard_product(pat_color, light.color);
}

// And lines 24-32 with:
if (material.pattern.at)
{
    t_tuple pat_color = pattern_color_at(material.pattern, position);
    result.ambient = hadamard_product(
        tuple_scalar_mult(pat_color, material.ambient),
        ambient_color);
}
```

**Also update shading_bonus.c** to use the same helper in `get_ambient_contribution()`.

**Risk Mitigated:** Code duplication, maintenance burden

---

### Fix #7: Handle Transform Inversion Failures Gracefully
**File:** `bonus/src/parsing/material_params_bonus.c`  
**Lines:** 57-78 (ptrans), 80-97 (pscale), 100-115 (prot_x/y/z)

**Pattern for All Transform Blocks:**
```c
if (ft_strncmp(param, "ptrans:", 7) == 0)
{
    parts = ft_split_rt(param + 7, ":");
    if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
        return (free_matrix(parts), false);
    if (!f_field_validation(parts[0]) || !f_field_validation(parts[1]) || !f_field_validation(parts[2]))
        return (free_matrix(parts), false);
    
    m = translation(ft_atof(parts[0]), ft_atof(parts[1]), ft_atof(parts[2]));
    pat->transform = multiply_matrix(pat->transform, m);
    
    // Check invertibility BEFORE setting has_transform
    if (!check_if_invertible(pat->transform))
    {
        // Error: transform matrix became singular
        pat->transform = identity();  // Reset to identity
        pat->has_transform = false;
        return (free_matrix(parts), false);  // Report error
    }
    
    pat->inv_transform = inverse_matrix(pat->transform);
    pat->has_transform = true;
    return (free_matrix(parts), true);
}
```

**Risk Mitigated:** Silent failures, debugging difficulty

---

## Priority 3: CODE QUALITY FIXES (Do Last)

### Fix #8: Add Comments to Pattern Color Initialization
**File:** `bonus/src/parsing/material_params_bonus.c`  
**Line:** 46

**Current:**
```c
dst->w = 0;
```

**Updated:**
```c
dst->w = 0;  // Ensure this is a color tuple, not a point
```

---

### Fix #9: Improve Rotation Angle Validation
**File:** `bonus/src/parsing/material_params_bonus.c`  
**Lines:** 100-122 (prot_x, prot_y, prot_z)

**Current:**
```c
if (ft_strncmp(param, "prot_x:", 7) == 0)
{
    if (!f_field_validation(param + 7))
        return (false);
    m = rotation_x(to_radians((int)ft_atof(param + 7)));
    // ...
}
```

**Improved (Optional):**
```c
if (ft_strncmp(param, "prot_x:", 7) == 0)
{
    float angle_deg;
    
    if (!f_field_validation(param + 7))
        return (false);
    angle_deg = ft_atof(param + 7);
    
    // Optional: Validate angle range
    if (angle_deg < -360.0f || angle_deg > 360.0f)
    {
        // Could return false, or normalize: angle_deg = fmod(angle_deg, 360.0f)
    }
    
    m = rotation_x(to_radians(angle_deg));  // Keep as float, no cast to int
    pat->transform = multiply_matrix(pat->transform, m);
    pat->has_transform = true;
    if (check_if_invertible(pat->transform))
        pat->inv_transform = inverse_matrix(pat->transform);
    else
        return (false);
    return (true);
}
```

---

## Testing Recommendations

After implementing fixes, test:

1. **Pattern Sampling:**
   - Test stripe_at() with negative coordinates
   - Test ring_at() with points at origin
   - Test checkers_at() at boundaries

2. **Light Brightness:**
   - Test with brightness = 0.0, 0.5, 1.0, 1.5 (should clamp)
   - Test with brightness = -1.0 (should clamp to 0.0)

3. **Pattern Transforms:**
   - Test singular matrix handling
   - Test chained transforms
   - Test pattern on transformed objects

4. **Input Validation:**
   - Test new_light() with out-of-range colors
   - Test new_light() with NaN/Infinity
   - Test pattern parsing with invalid parameters

5. **Memory Leaks:**
   - Run valgrind on pattern-heavy scenes
   - Check for matrix allocation/deallocation in transform chains

---

## Code Organization Summary

### Before (Current State)
- ❌ Uninitialized variables
- ❌ No input validation
- ❌ Inconsistent brightness application
- ❌ Duplicated pattern logic
- ❌ Silent failure modes

### After (Fixed State)
- ✅ All variables initialized
- ✅ Input validation on critical functions
- ✅ Consistent mathematical operations
- ✅ Centralized pattern sampling
- ✅ Error reporting instead of silent failures
