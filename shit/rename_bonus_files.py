#!/usr/bin/env python3
"""
Script to rename all files in bonus directory to have _bonus suffix
and update all #include statements accordingly.
"""

import os
import re
from pathlib import Path

# Define the bonus directory
BONUS_DIR = Path("/workspaces/miniRT/bonus")

# Mapping of old header names to new header names
header_mapping = {
    "core.h": "core_bonus.h",
    "execution.h": "execution_bonus.h",
    "geometry.h": "geometry_bonus.h",
    "lighting.h": "lighting_bonus.h",
    "math_ops.h": "math_ops_bonus.h",
    "miniRT.h": "miniRT_bonus.h",
    "parsing.h": "parsing_bonus.h",
    "rendering.h": "rendering_bonus.h",
    "types.h": "types_bonus.h",
    "utils.h": "utils_bonus.h",
}

def rename_headers():
    """Rename all header files in bonus/includes"""
    includes_dir = BONUS_DIR / "includes"
    print(f"Renaming header files in {includes_dir}...")
    
    for old_name, new_name in header_mapping.items():
        old_path = includes_dir / old_name
        new_path = includes_dir / new_name
        if old_path.exists():
            old_path.rename(new_path)
            print(f"  Renamed: {old_name} -> {new_name}")

def rename_source_files():
    """Rename all .c files in bonus/src to have _bonus suffix"""
    src_dir = BONUS_DIR / "src"
    print(f"\nRenaming source files in {src_dir}...")
    
    # Find all .c files recursively
    c_files = list(src_dir.rglob("*.c"))
    
    renamed_files = []
    for c_file in c_files:
        # Skip backup files
        if c_file.name.endswith('.backup') or c_file.name.endswith('.old'):
            continue
            
        # Get the new name with _bonus suffix
        old_name = c_file.name
        new_name = old_name.replace('.c', '_bonus.c')
        new_path = c_file.parent / new_name
        
        if old_name != new_name:
            c_file.rename(new_path)
            renamed_files.append((str(c_file), str(new_path)))
            print(f"  Renamed: {c_file.relative_to(BONUS_DIR)}")
    
    return renamed_files

def update_includes_in_file(file_path):
    """Update #include statements in a single file"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Update header includes
        for old_header, new_header in header_mapping.items():
            # Match various include patterns
            patterns = [
                (f'#include "{old_header}"', f'#include "{new_header}"'),
                (f'#include "../includes/{old_header}"', f'#include "../includes/{new_header}"'),
                (f'#include "../../includes/{old_header}"', f'#include "../../includes/{new_header}"'),
                (f'#include <{old_header}>', f'#include <{new_header}>'),
            ]
            
            for old_pattern, new_pattern in patterns:
                content = content.replace(old_pattern, new_pattern)
        
        # Only write if content changed
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        return False
    except Exception as e:
        print(f"  Error updating {file_path}: {e}")
        return False

def update_all_includes():
    """Update all #include statements in all source files"""
    print("\nUpdating #include statements in source files...")
    
    src_dir = BONUS_DIR / "src"
    c_files = list(src_dir.rglob("*.c"))
    
    updated_count = 0
    for c_file in c_files:
        if update_includes_in_file(c_file):
            updated_count += 1
            print(f"  Updated: {c_file.relative_to(BONUS_DIR)}")
    
    print(f"\nUpdated {updated_count} files")

def update_makefile():
    """Update the Makefile to reference _bonus files"""
    makefile_path = BONUS_DIR / "Makefile"
    print(f"\nUpdating {makefile_path}...")
    
    try:
        with open(makefile_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Replace .c with _bonus.c in SRCS variable
        # This regex finds paths like $(SRC_DIR)/path/file.c and replaces with file_bonus.c
        content = re.sub(r'(\$\(SRC_DIR\)/[^\s]*?)\.c', r'\1_bonus.c', content)
        
        if content != original_content:
            with open(makefile_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print("  Makefile updated")
        else:
            print("  Makefile unchanged")
    except Exception as e:
        print(f"  Error updating Makefile: {e}")

def main():
    print("=" * 60)
    print("Renaming bonus files to have _bonus suffix")
    print("=" * 60)
    
    # Step 1: Rename header files
    rename_headers()
    
    # Step 2: Rename source files
    rename_source_files()
    
    # Step 3: Update all #include statements
    update_all_includes()
    
    # Step 4: Update Makefile
    update_makefile()
    
    print("\n" + "=" * 60)
    print("Renaming complete!")
    print("=" * 60)

if __name__ == "__main__":
    main()
