# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amn700 <amn700@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by amn700            #+#    #+#              #
#    Updated: 2024/01/01 00:00:00 by amn700           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Global Makefile for miniRT project
# This Makefile can build both mandatory and bonus versions

.PHONY: all mandatory bonus clean fclean re help

# Default target - builds both mandatory and bonus
all: mandatory bonus

# Build mandatory version
mandatory:
	@echo "Building mandatory version..."
	@$(MAKE) -C mandatory

# Build bonus version
bonus:
	@echo "Building bonus version..."
	@$(MAKE) -C bonus

# Clean object files for both versions
clean:
	@echo "Cleaning object files..."
	@$(MAKE) -C mandatory clean
	@$(MAKE) -C bonus clean

# Full clean for both versions
fclean:
	@echo "Full clean..."
	@$(MAKE) -C mandatory fclean
	@$(MAKE) -C bonus fclean

# Rebuild everything
re: fclean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build both mandatory and bonus versions (default)"
	@echo "  mandatory - Build only mandatory version"
	@echo "  bonus     - Build only bonus version"
	@echo "  clean     - Remove object files from both versions"
	@echo "  fclean    - Remove all generated files from both versions"
	@echo "  re        - Clean and rebuild everything"
	@echo "  help      - Show this help message"