# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mac <mac@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by amn700            #+#    #+#              #
#    Updated: 2025/12/24 03:46:12 by mac              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = miniRT
#cc shot.c ../MLX42/build/libmlx42.a -lm -ldl -lglfw -pthread -lX11 -lGL -lrt -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
# Math library flag
MATH_FLAGS = -lm
# PNG library flags
PNG_CFLAGS = $(shell pkg-config --cflags libpng)
PNG_LIBS = $(shell pkg-config --libs libpng)
# macOS MLX42 flags for M1
# MLX_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit -lglfw
# Linux MLX42 flags (uncomment for Linux)
MLX_FLAGS = -ldl -lglfw -pthread -lX11 -lGL -lrt -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm
# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = ../libft
GNL_DIR = ../get_next_line
MLX42_DIR = ../MLX42

# Include paths
INCLUDES = -I$(INC_DIR) \
		   -I$(LIBFT_DIR) \
		   -I$(GNL_DIR) \
		   -I$(MLX42_DIR)/include \
		   $(PNG_CFLAGS)

# Source files
SRCS = $(SRC_DIR)/main.c \
	   $(SRC_DIR)/core/camera.c \
	   $(SRC_DIR)/core/ray_casting.c \
	   $(SRC_DIR)/core/shading.c \
	   $(SRC_DIR)/core/view_transform.c \
	   $(SRC_DIR)/core/world.c \
	   $(SRC_DIR)/core/computations.c \
	   $(SRC_DIR)/core/intersections.c \
	   $(SRC_DIR)/math/tuple_basic.c \
	   $(SRC_DIR)/math/tuple_arithmetic.c \
	   $(SRC_DIR)/math/vector_basic.c \
	   $(SRC_DIR)/math/vector_advanced.c \
	   $(SRC_DIR)/math/matrix_basic.c \
	   $(SRC_DIR)/math/matrix_multiply.c \
	   $(SRC_DIR)/math/matrix_submatrix.c \
	   $(SRC_DIR)/math/matrix_determinant.c \
	   $(SRC_DIR)/math/matrix_inverse.c \
	   $(SRC_DIR)/math/transformations_basic.c \
	   $(SRC_DIR)/math/transformations_rotation.c \
	   $(SRC_DIR)/math/transformations_shearing.c \
	   $(SRC_DIR)/math/axis_align.c \
	   $(SRC_DIR)/geometry/sphere.c \
	   $(SRC_DIR)/geometry/sphere_intersect.c \
	   $(SRC_DIR)/geometry/plane.c \
	   $(SRC_DIR)/geometry/cylinder_base.c \
	   $(SRC_DIR)/geometry/cylinder_intersect.c \
	   $(SRC_DIR)/geometry/ray_utils.c \
	   $(SRC_DIR)/geometry/intersections_utils.c \
	   $(SRC_DIR)/geometry/uv_mapping.c \
	   $(SRC_DIR)/lighting/phong_lighting.c \
	   $(SRC_DIR)/lighting/light_and_shades.c \
	   $(SRC_DIR)/lighting/texture_helpers.c \
	   $(SRC_DIR)/lighting/normals_helpers.c \
	   png/ft_png_read.c \
	   png/ft_png_img_to_tex.c \
	   png/ft_png_utils.c \
	   $(SRC_DIR)/rendering/render.c \
	   $(SRC_DIR)/rendering/image_export.c \
	   $(SRC_DIR)/rendering/color_conversion.c \
	   $(SRC_DIR)/rendering/coordinate_conversion.c \
	   $(SRC_DIR)/parsing/parsing.c \
	   $(SRC_DIR)/parsing/validators.c \
	   $(SRC_DIR)/parsing/field_parsers.c \
	   $(SRC_DIR)/parsing/ambient_camera_extract.c \
	   $(SRC_DIR)/parsing/light_extract.c \
	   $(SRC_DIR)/parsing/sphere_extract.c \
	   $(SRC_DIR)/parsing/plane_extract.c \
	   $(SRC_DIR)/parsing/cylinder_extract.c \
	   $(SRC_DIR)/parsing/scene_extractors.c \
	   $(SRC_DIR)/parsing/split.c \
	   $(SRC_DIR)/parsing/apply_ambient.c

# Get Next Line sources
GNL_SRCS = $(GNL_DIR)/get_next_line.c \
		   $(GNL_DIR)/get_next_line_utils.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
GNL_OBJS = $(GNL_SRCS:$(GNL_DIR)/%.c=$(OBJ_DIR)/%.o)

# Library files
LIBFT = $(LIBFT_DIR)/libft.a
MLX42 = ../MLX42/build/libmlx42.a
# LIBGLFW3 = ../MLX42/libglfw3.a  # macOS only, Linux uses -lglfw flag

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
NC = \033[0m # No Color

.PHONY: all clean fclean re help

# Default target
all: $(LIBFT) $(NAME)

# Create object directory structure
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/core
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/math
	@mkdir -p $(OBJ_DIR)/geometry
	@mkdir -p $(OBJ_DIR)/lighting
	@mkdir -p $(OBJ_DIR)/rendering
	@mkdir -p $(OBJ_DIR)/png

# Build libft
$(LIBFT):
	@echo "$(YELLOW)Building libft...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR)

# Object files compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/png/%.o: png/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Main executable
$(NAME): $(OBJS) $(GNL_OBJS) $(LIBFT)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(GNL_OBJS) $(LIBFT) $(MLX42) $(MLX_FLAGS) $(MATH_FLAGS) $(PNG_LIBS)
	@echo "$(GREEN)$(NAME) compiled successfully!$(NC)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

# Full clean
fclean: clean
	@echo "$(RED)Full clean...$(NC)"
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove object files"
	@echo "  fclean       - Remove all generated files"
	@echo "  re           - Clean and rebuild everything"
	@echo "  install-deps - Show command to install system dependencies"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Dependencies:"
	@echo "  - libft (built automatically)"
	@echo "  - get_next_line (compiled with project)"
	@echo "  - MLX42 (requires OpenGL: libglfw3-dev, libgl1-mesa-dev)"
	@echo "  - Math library (linked with -lm)"