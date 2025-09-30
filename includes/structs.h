/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:30:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 18:12:08 by serjimen         ###   ########.fr       */
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
 * @param key A dynamically allocated string for the variable name.
 * @param value A dynamically allocated string for the variable value.
 * @param full_env A dynamically allocated string for the full
 * "KEY=value".
 * @param next A pointer to the next node in the linked list.
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full_env;
	struct s_env	*next;
}					t_env;

/**
 * @enum e_token_type
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
	UNKNOWN,		/**< Represents a invalid command. */
}			t_token_type;

/**
 * @enum e_token_quote
 * @brief Enumeration for the different types of quotes.
 * This enum tracks whether the lexer is currently inside single quotes,
 * double quotes, or outside any quotes.
 */
typedef enum e_token_quote
{
	NONE,			/**< Outside any quoted context. */
	SINGLE,			/**< Inside single quotes. */
	DOUBLE,			/**< Inside double quotes */
}			t_token_quote;

/**
 * @struct t_token
 * @brief Structure for a single token in the command line.
 * This structure represents a single lexical token. It holds the string value
 * of the token, its classified type, and a pointer to the next token in the
 * sequence, forming a linked list. This linked list of token is the direct
 * output of the lexing phase and the primary input for the parsing phase.
 * @param value A dynamically allocated string containing the token's
 * literal value.
 * @param type The classified type of the token, as defined by the
 * 'e_token_type'.
 * @param next A pointer to the next token in the list. A NULL value
 * indicates the end of the token stream.
 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_token_quote	quote;
	bool			is_expanded;
	struct s_token	*next;
}					t_token;

/**
 * @struct t_redir
 * @brief Structure for command redirection information.
 * This structure stores information about input/output redirections
 * identified during parsing.
 * @param type Type of redirection.
 * @param file Dynamically allocated string for target filename or
 * heredoc delimiter.
 * @param next Pointer to the next redirection in the list.
 */
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int             heredoc_fd;
	struct s_redir	*next;
}					t_redir;

/**
 * @struct t_arg
 * @brief Node for a command argument list.
 * This structure holds a single command argument. The arguments for a command
 * are stored in a linked list. This design simplifies handling of variable
 * length argument lists during parsing.
 * @param arg A dynamically allocated string for the argument.
 * @param next A pointer to the next argument in the list.
 */
typedef struct s_arg
{
	char			*value;
	struct s_arg	*next;
}					t_arg;

/**
 * @struct t_command
 * @brief Structure for a parsed command unit.
 * This structure represents a single command identified by the parser.
 * @param args The head of a 't_arg' linked list containing all the
 * command's arguments.
 * @param cmd_argc The number of arguments in the 'args' list.
 * @param redirs The head of a 't_redir' linked list containing all
 * the commands redirections.
 * @param is_command Flag indicating if this an empty t_arg node.
 * @param next A pointer to the next command in the parsed command
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

/**
 * @struct t_parse_state
 * @brief State structure for the parsing process.
 * This structure maintains the current state during the parsing phase,
 * providing convenient access to the currently building command and
 * redirection nodes.
 * @param cmd_list Head of the complete command list being constructed.
 * @param cmd_node Pointer to the current command node being populated.
 * @param redir_node Pointer to the current redirection node being populated.
 */
typedef struct s_parse_state
{
	t_command			*cmd_list;
	t_command			*cmd_node;
	t_redir				*redir_node;
}						t_parse_state;

/**
 * @struct t_pipe
 * @brief Structure for pipeline execution context.
 * This structure contains all information needed to execute a pipeline of
 * commands, including file descriptors for input/output redirection and
 * environment context.
 * @param commands Head of the command list to execute.
 * @param env_list Pointer to the environment variables list.
 * @param command_count Total number of commands int he pipeline.
 * @param in Input file descriptor for the pipeline.
 * @param out Output file descriptor for the pipeline.
 * @param index Current position in the pipeline during execution.
 */
typedef struct s_pipe
{
	t_command	*commands;
	t_env		**env_list;
	int			command_count;
	int			in;
	int			out;
	int			index;
}				t_pipe;

typedef struct s_lexer
{
	t_token			*list;
	t_token			*new_token;
	t_token_quote	quote;
	bool			is_open;
	char			*buffer;
	char			*string;
	size_t			i;
	size_t			j;
	size_t			buffer_size;
}					t_lexer;

#endif