# 🎬 miniRT Scene File (.rt) Creation Guide

This comprehensive guide explains how to create valid `.rt` scene files for the miniRT ray tracer.

---

## 📋 Table of Contents

1. [File Format Basics](#file-format-basics)
2. [Mandatory Elements](#mandatory-elements)
   - [Ambient Light (A)](#ambient-light-a)
   - [Camera (C)](#camera-c)
   - [Light (L)](#light-l)
   - [Sphere (sp)](#sphere-sp)
   - [Plane (pl)](#plane-pl)
   - [Cylinder (cy)](#cylinder-cy)
3. [Bonus Elements](#bonus-elements)
   - [Cone (co)](#cone-co)
   - [Multiple Lights (l)](#multiple-lights-l)
   - [Material Parameters](#material-parameters)
   - [Textures](#textures)
   - [Patterns](#patterns)
4. [Complete Examples](#complete-examples)

---

## 📝 File Format Basics

### General Rules
- Each element is defined on a **single line**
- Fields are separated by **spaces** or **tabs**
- Coordinates and colors use **commas** (e.g., `0,1,0` or `255,128,64`)
- Lines starting with `#` are **comments** (ignored)
- Empty lines are **ignored**
- File extension must be `.rt`

### Value Types
| Type | Format | Example |
|------|--------|---------|
| Integer | Whole number | `255` |
| Float | Decimal number | `0.5`, `1.0` |
| Position | `x,y,z` | `0,5,-10` |
| Direction | `nx,ny,nz` (normalized -1 to 1) | `0,1,0` |
| Color | `R,G,B` (0-255) | `255,255,255` |

---

# 🔧 MANDATORY ELEMENTS

These elements are required and supported in the mandatory version.

---

## Ambient Light (A)

Defines global ambient lighting. **Required. Only one allowed.**

```
A <ratio> <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| ratio | float | 0.0 - 1.0 | Ambient light intensity |
| color | R,G,B | 0-255 each | Ambient light color |

**Example:**
```
A 0.3 255,255,255
```

---

## Camera (C)

Defines the viewpoint. **Required. Only one allowed.**

```
C <position> <direction> <fov>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| position | x,y,z | any float | Camera location in world |
| direction | nx,ny,nz | -1 to 1 | Normalized view direction |
| fov | integer | 0 - 180 | Horizontal field of view in degrees |

**Example:**
```
C 0,5,-15 0,-0.1,1 70
```

---

## Light (L)

Defines a point light source. **Required. Only one allowed in mandatory.**

```
L <position> <brightness> <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| position | x,y,z | any float | Light location in world |
| brightness | float | 0.0 - 1.0 | Light intensity |
| color | R,G,B | 0-255 each | Light color |

**Example:**
```
L 5,12,-5 0.9 255,255,255
```

---

## Sphere (sp)

Defines a sphere object.

```
sp <center> <diameter> <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| center | x,y,z | any float | Sphere center position |
| diameter | float | > 0 | Sphere diameter |
| color | R,G,B | 0-255 each | Surface color |

**Example:**
```
sp 0,2,0 3.0 200,100,50
```

---

## Plane (pl)

Defines an infinite plane.

```
pl <point> <normal> <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| point | x,y,z | any float | A point on the plane |
| normal | nx,ny,nz | -1 to 1 | Surface normal direction |
| color | R,G,B | 0-255 each | Surface color |

**Example:**
```
pl 0,0,0 0,1,0 150,150,150
```

---

## Cylinder (cy)

Defines a cylinder.

```
cy <center> <axis> <diameter> <height> <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| center | x,y,z | any float | Cylinder center position |
| axis | nx,ny,nz | -1 to 1 | Axis orientation (normalized) |
| diameter | float | > 0 | Cylinder diameter |
| height | float | > 0 | Cylinder height |
| color | R,G,B | 0-255 each | Surface color |

**Example:**
```
cy -5,0,6 0,1,0 2.0 4.0 139,90,43
```

---

# ⭐ BONUS ELEMENTS

Additional features available in the bonus version.

---

## Cone (co)

Defines a cone. **Bonus only.**

```
co <tip> <axis> <diameter> <height> [closed] <color>
```

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| tip | x,y,z | any float | Cone tip position |
| axis | nx,ny,nz | -1 to 1 | Axis orientation (normalized) |
| diameter | float | > 0 | Base diameter |
| height | float | > 0 | Cone height |
| closed | 0 or 1 | optional | 1 = closed base, 0 = open |
| color | R,G,B | 0-255 each | Surface color |

**Examples:**
```
# Open cone
co 6,0,4 0,1,0 2.0 3.5 128,128,128

# Closed cone
co 6,0,4 0,1,0 2.0 3.5 1 128,128,128
```

---

## Multiple Lights (l)

Additional point lights. Use lowercase `l`. **Bonus only.**

```
l <position> <brightness> <color>
```

Same format as `L`, but you can have multiple `l` lights.

**Example:**
```
L 5,12,-5 0.9 255,255,255      # Primary light (required)
l -5,10,5 0.6 255,200,150      # Additional warm light
l 0,8,-10 0.4 150,150,255      # Additional cool light
```

---

## Cylinder with Closed Caps (cy)

Bonus cylinders support closed caps.

```
cy <center> <axis> <diameter> <height> <closed> <color>
```

| Parameter | Value | Description |
|-----------|-------|-------------|
| closed | 0 | Open cylinder (tube) |
| closed | 1 | Closed caps on both ends |

**Example:**
```
cy -5,0,6 0,1,0 2.0 4.0 1 139,90,43
```

---

## Material Parameters

Optional parameters for advanced materials. Append after the color.

### Reflection (ref:)
Makes surfaces reflective (mirror-like).

```
ref:<value>
```
| Range | Description |
|-------|-------------|
| 0.0 | No reflection (default) |
| 1.0 | Perfect mirror |

**Example:**
```
sp 0,2,0 3.0 200,200,200 ref:0.8
```

---

### Refraction (refr:)
Makes surfaces transparent with light bending.

```
refr:<transparency>:<refract_index>
```

| Parameter | Range | Description |
|-----------|-------|-------------|
| transparency | 0.0 - 1.0 | How transparent (0 = opaque) |
| refract_index | 1.0 - 3.0 | Refractive index |

**Common refractive indices:**
| Material | Index |
|----------|-------|
| Air | 1.0 |
| Water | 1.33 |
| Glass | 1.5 - 1.52 |
| Diamond | 2.42 |

**Example:**
```
sp 0,2,0 3.0 200,200,200 refr:0.9:1.52
```

---

### Shininess (shine:)
Controls specular highlights.

```
shine:<shininess>
# or
shine:<specular>:<shininess>
```

| Parameter | Range | Description |
|-----------|-------|-------------|
| specular | 0.0 - 1.0 | Specular intensity |
| shininess | 1 - 1000 | Highlight tightness (higher = sharper) |

**Example:**
```
sp 0,2,0 3.0 200,200,200 shine:0.9:256
```

---

## Textures

Apply PNG texture images to objects.

### Color Texture (texture:)
Maps a color image onto the object's surface.
```
texture:<path_to_png>
```

**Example:**
```
sp 0,2,0 3.0 200,200,200 texture:../textures/wood.png
```

---

### Bump Map (bump:)

Bump maps create the **illusion of surface detail** without changing the geometry. They work by perturbing the surface normal based on a height map texture, which affects how light reflects off the surface.

#### How Bump Maps Work:

```
Normal Bump Map           Real Geometry
(Fake detail)             (Flat surface)

    Light                     Light
      ↓                         ↓
   ↙ ↓ ↘                        ↓
  ┌─┬─┬─┐                   ┌───────┐
  │ │ │ │  ← Perturbed      │       │  ← Actual flat
  └─┴─┴─┘    normals        └───────┘    surface
  
  Appears bumpy!            Still flat, but
                            shading varies
```

1. **Load grayscale image** - Brighter = higher, darker = lower
2. **Calculate height differences** - Sample adjacent pixels to get gradient
3. **Perturb normal vector** - Adjust based on gradient direction
4. **Light reacts differently** - Creates illusion of depth

#### Syntax:
```
bump:<path_to_png>
bump_strength:<value>
```

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| bump: | path | - | Path to grayscale or color PNG |
| bump_strength: | 0.0 - 1.0 | 0.5 | How pronounced the bumps appear |

#### Examples:
```
# Subtle bumps
sp 0,2,0 3.0 150,150,150 bump:../textures/brick.png bump_strength:0.3

# Strong bumps  
sp 0,2,0 3.0 150,150,150 bump:../textures/rocks.png bump_strength:0.8

# Combine with color texture (same image for realistic effect)
sp 0,2,0 3.0 180,180,180 texture:../textures/rocks.png bump:../textures/rocks.png bump_strength:0.6
```

#### Tips for Bump Maps:
- Use **grayscale images** for best results
- Higher `bump_strength` = more pronounced effect
- Works best with **diffuse lighting** (not too shiny)
- Combine with matching color texture for realism
- Silhouette remains smooth (geometry unchanged)

---

### Normal Map (normal:)

Normal maps are more advanced than bump maps. Instead of height values, they store **actual normal directions** in RGB channels:
- **Red** = X direction (-1 to 1)
- **Green** = Y direction (-1 to 1)  
- **Blue** = Z direction (-1 to 1)

Normal maps appear **blue-ish purple** because the default normal (0, 0, 1) maps to RGB (128, 128, 255).

```
normal:<path_to_png>
normal_strength:<0.0-1.0>
```

**Example:**
```
sp 0,2,0 3.0 200,200,200 normal:../textures/brick_normal.png normal_strength:0.8
```

#### Bump Map vs Normal Map:
| Feature | Bump Map | Normal Map |
|---------|----------|------------|
| Image type | Grayscale height | RGB directions |
| Appearance | Any colors | Blue-purple tint |
| Detail level | Good | Better |
| Performance | Faster | Slightly slower |
| Ease of creation | Easy (any image) | Requires tools |

---

## Patterns

Procedural patterns that don't require texture files.

### Pattern Type (pat:)
```
pat:<type>
```

| Type | Description |
|------|-------------|
| `stripe` | Alternating color stripes |
| `ring` | Concentric rings |
| `checkers` | 3D checkerboard pattern |

### Pattern Colors (pa:, pb:)
Define the two alternating colors using **colons** (not commas):
```
pa:<R>:<G>:<B>
pb:<R>:<G>:<B>
```

### Pattern Transform
Scale, translate, or rotate the pattern:
```
pscale:<x>:<y>:<z>      # Scale pattern
ptrans:<x>:<y>:<z>      # Translate pattern
prot_x:<degrees>        # Rotate around X axis
prot_y:<degrees>        # Rotate around Y axis
prot_z:<degrees>        # Rotate around Z axis
```

**Example:**
```
pl 0,0,0 0,1,0 150,150,150 pat:checkers pa:40:40:40 pb:220:220:220
```

---

# 📄 Complete Examples

## Minimal Scene (Mandatory)
```
# Minimal valid scene
A 0.3 255,255,255
C 0,5,-15 0,-0.1,1 70
L 5,12,-5 0.9 255,255,255
sp 0,2,0 3.0 200,100,50
```

## Showcase Scene (Bonus)
```
# Comprehensive showcase scene
A 0.3 255,255,255
C 0,8,-20 0,-0.05,1 70
L 8,15,-8 0.95 255,255,255
l -5,10,5 0.5 255,200,150

# Reflective checkerboard floor
pl 0,0,0 0,1,0 150,150,150 ref:0.5 pat:checkers pa:40:40:40 pb:220:220:220

# Glass sphere with refraction
sp 0,3.5,0 3.0 220,240,255 ref:0.8 refr:0.9:1.52 shine:0.9:512

# Metal sphere with high reflection
sp 8,2,-3 2.5 200,200,200 texture:../textures/metal.png ref:0.9 shine:0.95:512

# Textured wood cylinder with closed caps
cy -6,0,8 0,1,0 2.0 4.0 1 139,90,43 texture:../textures/wood.png

# Closed cone with rocks texture
co 7,0,7 0,1,0 2.5 4.0 1 128,128,128 texture:../textures/rocks.png
```

---

## ⚠️ Common Mistakes

| Error | Correct |
|-------|---------|
| `pa:50,50,50` | `pa:50:50:50` (use colons for pattern colors) |
| `refr:0.5` | `refr:0.5:1.5` (needs two values) |
| `ref:1.5` | `ref:1.0` (max is 1.0) |
| `C 0,0,0 0,0,0 70` | Direction can't be zero vector |
| Missing A, C, or L | All three are required |

---

## 🎨 Tips for Great Scenes

1. **Camera Position**: Place camera at negative Z looking toward positive Z
2. **Lighting**: Multiple lights create softer shadows
3. **Reflection**: Values 0.1-0.3 give subtle reflections, 0.8-1.0 for mirrors
4. **Glass**: Use `ref:0.1 refr:0.9:1.5` for realistic glass
5. **Metal**: Use `ref:0.8 shine:0.9:512` for shiny metal
6. **Floor**: Add `ref:0.2` for a polished floor look
7. **Patterns**: Scale patterns with `pscale:` to adjust size

---

*miniRT Ray Tracer - Scene File Guide v1.0*
