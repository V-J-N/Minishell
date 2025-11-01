# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/17 12:24:16 by vjan-nie          #+#    #+#              #
#    Updated: 2025/11/01 11:55:41 by vjan-nie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# Project

NAME	=		minishell

# **************************************************************************** #
# Paths

SRC_DIR =		src
INCLUDE_DIR =	includes
LIBFT_DIR =		libft
OBJ_DIR =		obj
CORE_DIR =		core
ENV_DIR =		env
LEXER_DIR =		lexer
PARSER_DIR =	parser
EXECUTOR_DIR =	executor
BUILTINS_DIR =	builtins
EXPANDER_DIR =	expander

# **************************************************************************** #
# Libft

LIBFT	=			$(LIBFT_DIR)/libft.a
LIBFT_INCLUDE	=	-I$(LIBFT_DIR)/include

# **************************************************************************** #
# Source files

SRC		=	$(SRC_DIR)/$(CORE_DIR)/minishell.c\
			$(SRC_DIR)/$(CORE_DIR)/minishell_utils.c\
			$(SRC_DIR)/$(CORE_DIR)/signals.c\
			$(SRC_DIR)/$(CORE_DIR)/global.c\
			$(SRC_DIR)/$(ENV_DIR)/environment.c\
			$(SRC_DIR)/$(ENV_DIR)/environment_utils.c\
			$(SRC_DIR)/$(ENV_DIR)/environment_utils2.c\
			$(SRC_DIR)/$(ENV_DIR)/environment_utils3.c\
			$(SRC_DIR)/$(ENV_DIR)/environment_utils4.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_tokens.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_tokens2.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_tokens3.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_tokens4.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_utils.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_utils2.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_utils3.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_utils4.c\
			$(SRC_DIR)/$(LEXER_DIR)/lexer_free.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_utils.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_cmd.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_redir.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_free.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_error.c\
			$(SRC_DIR)/$(PARSER_DIR)/parser_handler.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/executor.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/executor_utils.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/executor_utils2.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/redir.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/redir_utils.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/redir_utils2.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/here_doc.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/here_doc_utils.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/here_doc_utils2.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/pipes.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/pipes2.c\
			$(SRC_DIR)/$(EXECUTOR_DIR)/pipes_utils.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_utils.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_exit.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_echo.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_cd.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_export.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_unset_pwd_env.c\
			$(SRC_DIR)/$(BUILTINS_DIR)/builtins_export_utils.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_args.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_redirs.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_redirs2.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_utils.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_args2.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_utils2.c\
			$(SRC_DIR)/$(EXPANDER_DIR)/expander_utils3.c

# **************************************************************************** #
# Objects

OBJ		= 	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_DIRS =	$(OBJ_DIR)\
			$(OBJ_DIR)/core\
			$(OBJ_DIR)/env\
			$(OBJ_DIR)/lexer\
			$(OBJ_DIR)/parser\
			$(OBJ_DIR)/executor\
			$(OBJ_DIR)/builtins\
			$(OBJ_DIR)/expander\

# **************************************************************************** #
# Compiling rules

CC		= 	cc
CFLAGS	= 	-Wall -Wextra -Werror -g

# **************************************************************************** #
# Includes

INCLUDES =	-I$(INCLUDE_DIR) $(LIBFT_INCLUDE)

# **************************************************************************** #
# Colors

GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
BLUE	= \033[0;34m
RESET	= \033[0m

# **************************************************************************** #
# Progress variables
TOTAL_FILES := $(words $(SRC))
COMPILED_FILES = 0

# **************************************************************************** #
# Building commands:

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME) -lreadline
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) -s -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ libft compiled!$(RESET)"

$(OBJ_DIRS):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIRS)
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@printf "$(BLUE)[%2d/%2d]$(RESET) Compiling $<...\r" $(COMPILED_FILES) $(TOTAL_FILES)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIRS)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "$(GREEN)✓ Clean complete!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Full clean complete!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
