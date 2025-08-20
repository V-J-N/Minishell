# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/17 12:24:16 by vjan-nie          #+#    #+#              #
#    Updated: 2025/08/20 12:05:48 by serjimen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 	minishell

# Paths

SRC_DIR = src

INCLUDE_DIR = includes

LIBFT_DIR = libft

OBJ_DIR = obj

# Libft

LIBFT	=	$(LIBFT_DIR)/libft.a

LIBFT_INCLUDE	= -I$(LIBFT_DIR)/include

# Source files

SRC		=	$(SRC_DIR)/minishell.c\
			$(SRC_DIR)/minishell_utils.c\
			$(SRC_DIR)/executor.c\
			$(SRC_DIR)/executor_utils.c\
			$(SRC_DIR)/executor_utils2.c\
			$(SRC_DIR)/environment.c\
			$(SRC_DIR)/environment_utils.c\
			$(SRC_DIR)/environment_memory.c

# Objects

OBJ		= 	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiling rules

CC		= 	cc

CFLAGS	= 	-Wall -Wextra -Werror -g

# Includes

INCLUDES = -I$(INCLUDE_DIR) $(LIBFT_INCLUDE)

# Building commands:

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
