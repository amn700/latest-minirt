#!/bin/bash
# Script to rename all files in bonus directory to have _bonus suffix
# and update all #include statements accordingly

set -e

BONUS_DIR="/workspaces/miniRT/bonus"

echo "============================================================"
echo "Renaming bonus files to have _bonus suffix"
echo "============================================================"

# Step 1: Rename header files in bonus/includes
echo ""
echo "Step 1: Renaming header files..."
cd "$BONUS_DIR/includes"

for file in *.h; do
    if [ -f "$file" ] && [[ ! "$file" =~ _bonus\.h$ ]]; then
        newname="${file%.h}_bonus.h"
        mv "$file" "$newname"
        echo "  Renamed: $file -> $newname"
    fi
done

# Step 2: Rename source files in bonus/src
echo ""
echo "Step 2: Renaming source files..."
cd "$BONUS_DIR/src"

# Find all .c files and rename them
find . -name "*.c" ! -name "*_bonus.c" ! -name "*.backup" ! -name "*.old" -type f | while read -r file; do
    newname="${file%.c}_bonus.c"
    mv "$file" "$newname"
    echo "  Renamed: $file"
done

# Step 3: Update all #include statements in source files
echo ""
echo "Step 3: Updating #include statements in source files..."
cd "$BONUS_DIR/src"

find . -name "*_bonus.c" -type f | while read -r file; do
    # Update header includes
    sed -i 's/#include "core\.h"/#include "core_bonus.h"/g' "$file"
    sed -i 's/#include "execution\.h"/#include "execution_bonus.h"/g' "$file"
    sed -i 's/#include "geometry\.h"/#include "geometry_bonus.h"/g' "$file"
    sed -i 's/#include "lighting\.h"/#include "lighting_bonus.h"/g' "$file"
    sed -i 's/#include "math_ops\.h"/#include "math_ops_bonus.h"/g' "$file"
    sed -i 's/#include "miniRT\.h"/#include "miniRT_bonus.h"/g' "$file"
    sed -i 's/#include "parsing\.h"/#include "parsing_bonus.h"/g' "$file"
    sed -i 's/#include "rendering\.h"/#include "rendering_bonus.h"/g' "$file"
    sed -i 's/#include "types\.h"/#include "types_bonus.h"/g' "$file"
    sed -i 's/#include "utils\.h"/#include "utils_bonus.h"/g' "$file"
    
    # Update includes with relative paths
    sed -i 's|#include "../includes/core\.h"|#include "../includes/core_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/execution\.h"|#include "../includes/execution_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/geometry\.h"|#include "../includes/geometry_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/lighting\.h"|#include "../includes/lighting_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/math_ops\.h"|#include "../includes/math_ops_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/miniRT\.h"|#include "../includes/miniRT_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/parsing\.h"|#include "../includes/parsing_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/rendering\.h"|#include "../includes/rendering_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/types\.h"|#include "../includes/types_bonus.h"|g' "$file"
    sed -i 's|#include "../includes/utils\.h"|#include "../includes/utils_bonus.h"|g' "$file"
    
    # Update includes with ../../includes/ paths
    sed -i 's|#include "../../includes/core\.h"|#include "../../includes/core_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/execution\.h"|#include "../../includes/execution_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/geometry\.h"|#include "../../includes/geometry_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/lighting\.h"|#include "../../includes/lighting_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/math_ops\.h"|#include "../../includes/math_ops_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/miniRT\.h"|#include "../../includes/miniRT_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/parsing\.h"|#include "../../includes/parsing_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/rendering\.h"|#include "../../includes/rendering_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/types\.h"|#include "../../includes/types_bonus.h"|g' "$file"
    sed -i 's|#include "../../includes/utils\.h"|#include "../../includes/utils_bonus.h"|g' "$file"
    
    echo "  Updated: $file"
done

# Step 4: Update header includes within header files
echo ""
echo "Step 4: Updating #include statements in header files..."
cd "$BONUS_DIR/includes"

for file in *_bonus.h; do
    if [ -f "$file" ]; then
        sed -i 's/#include "core\.h"/#include "core_bonus.h"/g' "$file"
        sed -i 's/#include "execution\.h"/#include "execution_bonus.h"/g' "$file"
        sed -i 's/#include "geometry\.h"/#include "geometry_bonus.h"/g' "$file"
        sed -i 's/#include "lighting\.h"/#include "lighting_bonus.h"/g' "$file"
        sed -i 's/#include "math_ops\.h"/#include "math_ops_bonus.h"/g' "$file"
        sed -i 's/#include "miniRT\.h"/#include "miniRT_bonus.h"/g' "$file"
        sed -i 's/#include "parsing\.h"/#include "parsing_bonus.h"/g' "$file"
        sed -i 's/#include "rendering\.h"/#include "rendering_bonus.h"/g' "$file"
        sed -i 's/#include "types\.h"/#include "types_bonus.h"/g' "$file"
        sed -i 's/#include "utils\.h"/#include "utils_bonus.h"/g' "$file"
        echo "  Updated: $file"
    fi
done

# Step 5: Update the Makefile
echo ""
echo "Step 5: Updating Makefile..."
cd "$BONUS_DIR"

if [ -f "Makefile" ]; then
    # Replace .c with _bonus.c in source file paths
    sed -i 's/\($(SRC_DIR)\/[^ ]*\)\.c/\1_bonus.c/g' Makefile
    echo "  Makefile updated"
fi

echo ""
echo "============================================================"
echo "Renaming complete!"
echo "============================================================"
