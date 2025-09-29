# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/17 12:24:16 by vjan-nie          #+#    #+#              #
#    Updated: 2025/09/29 05:32:45 by vjan-nie         ###   ########.fr        #
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
			$(SRC_DIR)/global.c\
			$(SRC_DIR)/environment.c\
			$(SRC_DIR)/environment_utils.c\
			$(SRC_DIR)/environment_utils2.c\
			$(SRC_DIR)/environment_utils3.c\
			$(SRC_DIR)/lexer.c\
			$(SRC_DIR)/lexer_tokens.c\
			$(SRC_DIR)/lexer_tokens2.c\
			$(SRC_DIR)/lexer_tokens3.c\
			$(SRC_DIR)/lexer_utils.c\
			$(SRC_DIR)/lexer_utils2.c\
			$(SRC_DIR)/lexer_utils3.c\
			$(SRC_DIR)/lexer_test.c\
			$(SRC_DIR)/parser.c\
			$(SRC_DIR)/parser_utils.c\
			$(SRC_DIR)/parser_cmd.c\
			$(SRC_DIR)/parser_redir.c\
			$(SRC_DIR)/parser_free.c\
			$(SRC_DIR)/parser_error.c\
			$(SRC_DIR)/parser_handler.c\
			$(SRC_DIR)/parser_test.c\
			$(SRC_DIR)/executor.c\
			$(SRC_DIR)/executor_utils.c\
			$(SRC_DIR)/executor_utils2.c\
			$(SRC_DIR)/redir.c\
			$(SRC_DIR)/redir_utils.c\
			$(SRC_DIR)/here_doc.c\
			$(SRC_DIR)/builtins.c\
			$(SRC_DIR)/builtins_utils.c\
			$(SRC_DIR)/builtins_exit.c\
			$(SRC_DIR)/builtins_echo.c\
			$(SRC_DIR)/builtins_cd.c\
			$(SRC_DIR)/builtins_export.c\
			$(SRC_DIR)/builtins_export_utils.c\
			$(SRC_DIR)/pipes.c\
			$(SRC_DIR)/pipes_utils.c

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
