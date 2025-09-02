/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:30:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/02 16:40:25 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file strutcs.h
 * @brief Header file for minishell data structures.
 */
#ifndef STRUCTS_H_
# define STRUCTS_H_

/**
 * @struct t_env
 * @brief Nodo de la lista enlazada que representa un variablle de entorno.
 * 
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
	HEREDOC			/**< Represents the '<<' symbol for here-document. */
}			t_token_type;

/**
 * @brief Structure for a single token in the command line.
 * This structure represents a single lexical token. It holds the string value
 * of the token,, its classified type, and a pointer to the next token in the
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
	struct s_token	*next;
}				t_token;

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