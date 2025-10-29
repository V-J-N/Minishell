/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:30:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/29 11:56:07 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file structs.h
 * @brief Header file for minishell data structures.
 * This file contains the definitions for all core data structures used
 * throughout the minishell project.
 */
#ifndef STRUCTS_H
# define STRUCTS_H

/* ************************************************************************** */
/* Enumerations */

/**
 * @enum e_token_type
 * @brief Enumeration for the different types of tokens recognized for the
 * lexer.
 * This enum defines the possible categories a token can fall into, which
 * the parser uses to build the command execution.
 */
typedef enum e_token_type
{
	WORD,			/**< A sequence of characters that is not an operator. */
	PIPE,			/**< Represents the '|' symbol for piping commands. */
	REDIR_IN,		/**< Represents the '<' symbol for input redirection. */
	REDIR_OUT,		/**< Represents the '>' symbol for output redirection. */
	APPEND,			/**< Represents the '>>' symbol for appending output. */
	HEREDOC,		/**< Represents the '<<' symbol for here-document. */
}			t_token_type;

/**
 * @enum e_token_quote
 * @brief Tracks the quoting status associated with a token's value
 * This enum determines wheter a token's content should be expanded during
 * the expasion phase.
 */
typedef enum e_token_quote
{
	NONE,			/**< The token value was unquoted. */
	SINGLE,			/**< The token value was entirely or partially
					enclosed in single quotes. */
	DOUBLE,			/**< The token value was entirely or partially
					enclosed in double quotes */
}			t_token_quote;

/**
 * @enum e_token_state
 * @brief Defines the current state of the lexer's character processing loop.
 * Used internally by the lexer to determine how to process the next character
 * based on whether it is inside or outside quotes.
 */
typedef enum e_token_state
{
	OUT,			/**< Outside any quoted context. */
	IN_SINGLE,		/**< Inside single quotes. */
	IN_DOUBLE,		/**< Inside double quotes. */
}			t_token_state;

/* ************************************************************************** */
/* Core Structures */

/**
 * @struct t_env
 * @brief Node for the environment variable linked list.
 * This structure represents a single environment variable, storing its
 * components in a deep-copied format to ensure independece from the original
 * system environment.
 * @param key A dynamically allocated string for the variable name.
 * @param value A dynamically allocated string for the variable value.
 * @param full_env A dynamically allocated string for the full
 * "KEY=value" format.
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
 * @struct t_token
 * @brief Structure for a single lexical token from the input line.
 * This linked list of tokens is the direct output of the lexing phase and the
 * primary input for the parsing phase.
 * @param value A dynamically allocated string containing the token's
 * literal value.
 * @param type The classified type of the token, as defined by the
 * 'e_token_type'.
 * @param quote The type of quote that ultimately wrapped the token's value.
 * @param is_expanded Boolean flag: true if variables should be expanded.
 * @param has_quotes Boolean flag: true if the token value originally contained
 * any quotes, used for expasion/removal logic.
 * @param next A pointer to the next token in the list. A NULL value
 * indicates the end of the token stream.
 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_token_quote	quote;
	bool			is_expanded;
	bool			has_quotes;
	struct s_token	*next;
}					t_token;

/**
 * @struct t_redir
 * @brief Structure for command redirection information.
 * This structure stores all details about input/output redirections
 * for a command. The extra fields are used internally during the variable
 * expasion phase.
 * @param quote The final quoting context of the file field.
 * @param type The type of redirection operator.
 * @param file Dynamically allocated string for target filename or
 * heredoc delimiter.
 * @param env_file Reserved for storing the environment variable.
 * @param exp_file Reserved for storing the string after environment
 * variable expasion.
 * @param is_expanded Boolean flag: true if environment variable expasion
 * is allowed.
 * @param has_quotes Boolean flag: true if the file name originally contained
 * quotes.
 * @param heredoc_fd File descriptor for the temporary file created for
 * a HEREDOC, if appplicable (or -1);
 * @param i Index used by the expander to track position within the file
 * string.
 * @param next Pointer to the next redirection in the list.
 */
typedef struct s_redir
{
	t_token_quote	quote;
	t_token_type	type;
	char			*file;
	char			*env_file;
	char			*exp_file;
	bool			is_expanded;
	bool			has_quotes;
	int				heredoc_fd;
	size_t			i;
	struct s_redir	*next;
}					t_redir;

/**
 * @struct t_arg
 * @brief Node for a command argument list.
 * This structure holds a single command argument. The arguments for a command
 * are stored in a linked list. This design simplifies handling of variable
 * length argument lists during parsing. The extra field are used internally
 * during the variable expansion phase.
 * @param quote The final quoting context of the argument's value.
 * @param value Dynamically allocated string for the original argument.
 * @param env_value Reserved for storing the environment variable.
 * @param exp_value Reserved for storing the string after environment variable
 * expasion.
 * @param is_expanded Boolean flag: true if environment variable expasion
 * is allowed.
 * @param has_quotes Boolean flag: true if the argument originally contained
 * quotes.
 * @param i Index used by the expander to track position within the argument
 * string.
 * @param next A pointer to the next argument in the list.
 */
typedef struct s_arg
{
	t_token_quote	quote;
	char			*value;
	char			*env_value;
	char			*exp_value;
	bool			is_expanded;
	bool			has_quotes;
	size_t			i;
	struct s_arg	*next;
}					t_arg;

/**
 * @struct t_command
 * @brief Structure for a parsed command unit.
 * This structure represents one command, including its arguments and
 * redirections, as identified by the parser.
 * @param args The head of a 't_arg' linked list containing the command
 * and all arguments.
 * @param cmd_argc The number of arguments in the 'args' list.
 * @param redirs The head of a 't_redir' linked list containing all
 * the commands redirections.
 * @param is_command Boolean Flag: true if the command name has been identified
 * and stored; false if the node is only holding redirections.
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
 * providing convenient access to the overall result and the nodes currently
 * being built.
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
 * @struct t_data
 * @brief Global container for all main shell data structures.
 * This acts as the central data object passed between major components.
 * @param env Head of the environment variable list.
 * @param token Head of the current token list.
 * @param parsed The parser state structure, holding the command pipeline.
 */
typedef struct s_data
{
	t_env			*env;
	t_token			*token;
	t_parse_state	*parsed;
}					t_data;

/**
 * @struct t_pipe
 * @brief Structure for pipeline execution context.
 * This structure contains all information needed to execute a pipeline of
 * commands, including file descriptors for input/output redirection and
 * environment context.
 * @param commands Head of the command list to execute.
 * @param current_command Pointer to the command currently being executed in
 * the pipeline.
 * @param env_list Pointer to the environment variables list.
 * @param command_count Total number of commands int he pipeline.
 * @param in Input file descriptor for the pipeline.
 * @param out Output file descriptor for the pipeline.
 * @param index Current position in the pipeline during execution.
 * @param data Pointer to the main global data structure.
 */
typedef struct s_pipe
{
	t_command	*commands;
	t_command	*current_command;
	t_env		**env_list;
	int			command_count;
	int			in;
	int			out;
	int			index;
	t_data		*data;
}				t_pipe;

/**
 * @struct t_lexer
 * @brief State structure for the lexer.
 * This structure holds the entire state required to convert the input string
 * into a stream of tokens character by character.
 * @param list Head of the 't_token' list being constructed.
 * @param new_token Pointer to the token currnetly being finalized.
 * @param quote Tracks the last quote type encountered.
 * @param state Tracks the current quoting state.
 * @param buffer Dynamically growing string buffer used to accumulated
 * character for a new token.
 * @param string The original input command line string.
 * @param i Index tracking the current position in the input 'string'.
 * @param j Index tracking the current position in the 'buffer'.
 * @param buffer_size The allocated size of the buffer.
 * @param has_quotes Boolean flag: true if the current word being built contained
 * any quotes.
 */
typedef struct s_lexer
{
	t_token			*list;
	t_token			*new_token;
	t_token_quote	quote;
	t_token_state	state;
	char			*buffer;
	char			*string;
	size_t			i;
	size_t			j;
	size_t			buffer_size;
	bool			has_quotes;
}					t_lexer;

#endif