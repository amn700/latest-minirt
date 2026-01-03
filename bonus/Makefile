# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amn <amn@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by amn700            #+#    #+#              #
#    Updated: 2026/01/03 07:16:17 by amn              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = miniRT_bonus

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
# Math library flag
MATH_FLAGS = -lm

# macOS MLX42 flags for M1
# MLX_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit -L/goinfre/mohchaib/homebrew/lib -lglfw -pthread -ldl
# Linux MLX42 flags (uncomment for Linux)
MLX_FLAGS = -ldl -lglfw -pthread -lm
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
		   -I$(MLX42_DIR)/include

# Source files
SRCS = $(SRC_DIR)/main_bonus.c \
	   $(SRC_DIR)/core/camera_bonus.c \
	   $(SRC_DIR)/core/ray_casting_bonus.c \
	   $(SRC_DIR)/core/shading_bonus.c \
	   $(SRC_DIR)/core/view_transform_bonus.c \
	   $(SRC_DIR)/core/world_bonus.c \
	   $(SRC_DIR)/core/computations_bonus.c \
	   $(SRC_DIR)/core/intersections_bonus.c \
	   $(SRC_DIR)/core/refraction_helpers_bonus.c \
	   $(SRC_DIR)/math/tuple_basic_bonus.c \
	   $(SRC_DIR)/math/tuple_arithmetic_bonus.c \
	   $(SRC_DIR)/math/vector_basic_bonus.c \
	   $(SRC_DIR)/math/vector_advanced_bonus.c \
	   $(SRC_DIR)/math/matrix_basic_bonus.c \
	   $(SRC_DIR)/math/matrix_multiply_bonus.c \
	   $(SRC_DIR)/math/matrix_submatrix_bonus.c \
	   $(SRC_DIR)/math/matrix_determinant_bonus.c \
	   $(SRC_DIR)/math/matrix_inverse_bonus.c \
	   $(SRC_DIR)/math/transformations_basic_bonus.c \
	   $(SRC_DIR)/math/transformations_rotation_bonus.c \
	   $(SRC_DIR)/math/transformations_shearing_bonus.c \
	   $(SRC_DIR)/math/axis_align_bonus.c \
	   $(SRC_DIR)/geometry/sphere_bonus.c \
	   $(SRC_DIR)/geometry/sphere_intersect_bonus.c \
	   $(SRC_DIR)/geometry/plane_bonus.c \
	   $(SRC_DIR)/geometry/cylinder_base_bonus.c \
	   $(SRC_DIR)/geometry/cylinder_intersect_bonus.c \
	   $(SRC_DIR)/geometry/cone_base_bonus.c \
	   $(SRC_DIR)/geometry/cone_intersect_bonus.c \
	   $(SRC_DIR)/geometry/ray_utils_bonus.c \
	   $(SRC_DIR)/geometry/intersections_utils_bonus.c \
	   $(SRC_DIR)/lighting/phong_lighting_bonus.c \
	   $(SRC_DIR)/lighting/light_and_shades_bonus.c \
	   $(SRC_DIR)/rendering/render_bonus.c \
	   $(SRC_DIR)/rendering/image_export_bonus.c \
	   $(SRC_DIR)/rendering/ui_bonus.c \
	   $(SRC_DIR)/parsing/parsing_bonus.c \
	   $(SRC_DIR)/parsing/validators_bonus.c \
	   $(SRC_DIR)/parsing/field_parsers_bonus.c \
	   $(SRC_DIR)/parsing/ambient_camera_extract_bonus.c \
	   $(SRC_DIR)/parsing/light_extract_bonus.c \
	   $(SRC_DIR)/parsing/sphere_extract_bonus.c \
	   $(SRC_DIR)/parsing/plane_extract_bonus.c \
	   $(SRC_DIR)/parsing/cylinder_extract_bonus.c \
	   $(SRC_DIR)/parsing/cone_extract_bonus.c \
	   $(SRC_DIR)/parsing/scene_extractors_bonus.c \
	   $(SRC_DIR)/parsing/split_bonus.c \
	   $(SRC_DIR)/parsing/apply_ambient_bonus.c \
	   $(SRC_DIR)/parsing/debug_parsing_bonus.c \
	   $(SRC_DIR)/parsing/material_params_bonus.c \
	   $(SRC_DIR)/parsing/texture_parsing_bonus.c \
	   $(SRC_DIR)/geometry/uv_mapping_bonus.c \
	   $(SRC_DIR)/lighting/bump_mapping_bonus.c

# Get Next Line sources
GNL_SRCS = $(GNL_DIR)/get_next_line.c \
		   $(GNL_DIR)/get_next_line_utils.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%_bonus.c=$(OBJ_DIR)/%.o)
GNL_OBJS = $(GNL_SRCS:$(GNL_DIR)/%.c=$(OBJ_DIR)/%.o)
EXTRA_SRCS = $(SRC_DIR)/paterns/patern.c
EXTRA_OBJS = $(EXTRA_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

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
	@mkdir -p $(OBJ_DIR)/paterns

# Build libft
$(LIBFT):
	@echo "$(YELLOW)Building libft...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR)

# Object files compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%_bonus.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<$(NC)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Main executable
$(NAME): $(OBJS) $(EXTRA_OBJS) $(GNL_OBJS) $(LIBFT)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(EXTRA_OBJS) $(GNL_OBJS) $(LIBFT) $(MLX42) $(MLX_FLAGS) $(MATH_FLAGS)
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

# ----------------------
# Test targets
# ----------------------
.PHONY: test_cones

TESTS_DIR := tests
TESTS_BIN := cone_tests

TEST_SRCS := $(TESTS_DIR)/cone_tests.c
TEST_OBJS := $(TEST_SRCS:%.c=$(OBJ_DIR)/%.o)

TEST_INCLUDES := -I$(INC_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $< (tests)$(NC)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(TEST_INCLUDES) -c $< -o $@

# Build only the minimal set of objects needed for cone tests (no MLX)
TEST_MIN_OBJS := \
	$(OBJ_DIR)/math/tuple_basic_bonus.o \
	$(OBJ_DIR)/math/tuple_arithmetic_bonus.o \
	$(OBJ_DIR)/math/vector_basic_bonus.o \
	$(OBJ_DIR)/math/vector_advanced_bonus.o \
	$(OBJ_DIR)/math/matrix_basic_bonus.o \
	$(OBJ_DIR)/math/matrix_multiply_bonus.o \
	$(OBJ_DIR)/math/matrix_submatrix_bonus.o \
	$(OBJ_DIR)/math/matrix_determinant_bonus.o \
	$(OBJ_DIR)/math/matrix_inverse_bonus.o \
	$(OBJ_DIR)/math/transformations_basic_bonus.o \
	$(OBJ_DIR)/math/axis_align_bonus.o \
	$(OBJ_DIR)/geometry/ray_utils_bonus.o \
	$(OBJ_DIR)/geometry/intersections_utils_bonus.o \
	$(OBJ_DIR)/geometry/cone_base_bonus.o \
	$(OBJ_DIR)/geometry/cone_intersect_bonus.o \
	$(OBJ_DIR)/geometry/sphere_bonus.o \
	$(OBJ_DIR)/geometry/sphere_intersect_bonus.o \
	$(OBJ_DIR)/geometry/plane_bonus.o

test_cones: $(LIBFT) $(TEST_OBJS) $(TEST_MIN_OBJS)
	@echo "$(YELLOW)Linking $(TESTS_BIN)...$(NC)"
	@$(CC) $(CFLAGS) -o $(TESTS_BIN) $(TEST_OBJS) $(TEST_MIN_OBJS) $(LIBFT) $(MATH_FLAGS)
	@echo "$(GREEN)Run ./$(TESTS_BIN) to execute tests$(NC)"