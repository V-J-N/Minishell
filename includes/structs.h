/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:30:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/09 13:09:16 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file strutcs.h
 * @brief Header file for minishell data structures.
 * This file contains the definitions for all core data structures used
 * throughout the minishell project.
 */
#ifndef STRUCTS_H
# define STRUCTS_H

/**
 * @struct t_env
 * @brief Node for the environment variable linked list.
 * This structure represents a single environment variable, storing its
 * components in a deep-copied format.
 * @var s_env::key A dynamically allocated string for the variable name.
 * @var s_env::value A dynamically allocated string for the variable value.
 * @var s_env::full_env A dynamically allocated string for the full
 * "KEY=value".
 * @var s_env::next A pointer to the next node in the linked list.
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full_env;
	struct s_env	*next;
}					t_env;

/**
 * @brief Enumeration for the different types of tokens.
 * This enum defines the possible categories a token can fall into.
 * The lexer will use these types to classify each word and operator from
 * the input string, which the parser will later use to build the command
 * execution tree.
 */
typedef enum e_token_type
{
	WORD,			/**< A sequence of characters that is not an operator. */
	PIPE,			/**< Represents the '|' symbol for piping commands. */
	REDIR_IN,		/**< Represents the '<' symbol for input redirection. */
	REDIR_OUT,		/**< Represents the '>' symbol for output redirection. */
	APPEND,			/**< Represents the '>>' symbol for appending output. */
	HEREDOC,		/**< Represents the '<<' symbol for here-document. */
	UNKNOWN			/**< Represents a invalid command. */
}			t_token_type;

/**
 * @brief Structure for a single token in the command line.
 * This structure represents a single lexical token. It holds the string value
 * of the token, its classified type, and a pointer to the next token in the
 * sequence, forming a linked list. This linked list of token is the direct
 * output of the lexing phase and the primary input for the parsing phase.
 * @var s_token::value A dynamically allocated string containing the token's
 * literal value.
 * @var s_token::type The classified type of the token, as defined by the
 * 'e_token_type'.
 * @var s_token::next A pointer to the next token in the list. A NULL value
 * indicates the end of the token stream.
 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	// size_t	pos; // Saber la posición de los tokens, para gestión de errores.
	// struct s_token	*prev; // <- Establecer una lista doblemente enlazada en el lexer.
	struct s_token	*next;
}					t_token;

/**
 * @brief
 */
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

/**
 * @brief Node for a command argument list.
 * This structure holds a single command argument. The arguments for a command
 * are stored in a linked list. This design simplifies handling of variable
 * length argument lists during parsing.
 * @var t_arg::arg A dynamically allocated string for the argument.
 * @var t_arg::next A pointer to the next argument in the list.
 */
typedef struct s_arg
{
	char			*value;
	struct s_arg	*next;
}					t_arg;

/**
 * @brief Structure for a parsed command.
 * This structure represents a single command identified by the parser.
 * @var s_command::args The head of a 't_arg' linked list containing all the
 * command's arguments.
 * @var s_command::cmd_argc The number of arguments in the 'args' list.
 * @var s_command::redirs The head of a 't_redir' linked list containing all
 * the commands redirections.
 * @var s_command::next A pointer to the next command in the parsed command
 * stream.
 */
typedef struct s_command
{
	t_arg				*args;
	size_t				cmd_argc;
	t_redir				*redirs;
	bool				is_command;
	struct s_command	*next;
}						t_command;

typedef struct s_pipe
{
	char	**args;
	int		blocks;
	t_env **env_list;
	int		in;
	int		out;
	int		index;
}			t_pipe;


#endif