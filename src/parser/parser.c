/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 20:44:46 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main logic and orchestation for converting a token stream into an
 * executable command pipeline.
 */
#include "minishell.h"

/**
 * @brief Allocates and initializes the parser state structure.
 * This function creates a new 't_parse_state' structure, which serves
 * serves as a central container for all data generated during the parsing
 * process.
 * @return t_parse_state* A pointer to the newly allocated and initialized
 * 't_parse_state' structure. Returns 'NULL' if memory allocation fails.
 */
static t_parse_state	*init_parser(void)
{
	t_parse_state	*parser;

	parser = ft_calloc(1, sizeof(t_parse_state));
	if (!parser)
		return (NULL);
	parser->cmd_list = NULL;
	parser->cmd_node = NULL;
	parser->redir_node = NULL;
	return (parser);
}

/**
 * @brief Main dispatcher loop that processes the token stream based on the
 * token type.
 * This function drives the state machine of the parser. It iterates through
 * the token list and delegates the processing of each token type to
 * dedicated handler functions.
 * @param tokens The current head of the 't_token' list segment to be processed.
 * @param p_struct A pointer to the central 't_parse_state' structure.
 * @return true If the entire token stream was processed successfully.
 * @return false If a syntax error or fatal allocation error occurred.
 */
static bool	handle_tokens(t_token *tokens, t_parse_state *p_struct)
{
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if (!handle_word(tokens, p_struct))
				return (free_tokens(&tokens),
					parse_error("minishell:WORD parse error", p_struct), NULL);
			tokens = tokens->next;
		}
		else if (tokens->type == PIPE)
		{
			if (!handle_pipe(tokens, p_struct))
				return (free_tokens(&tokens),
					parse_error("minishell:PIPE parse error", p_struct), NULL);
			tokens = tokens->next;
		}
		else if (is_redir(tokens))
		{
			if (!handle_redir(tokens, p_struct))
				return (free_tokens(&tokens),
					parse_error("minishell:REDIR parse error", p_struct), NULL);
			tokens = tokens->next->next;
		}
	}
	return (true);
}

/**
 * @brief Parses a token stream to create a command block list.
 * This function is the main public interface for the parsing phase. It
 * orchestates the entire process from initialization to final list asembly.
 * @param tokens A pointer to the head of the 't_token' linked list, which
 * represents the command line to be parsed.
 * @return t_parse_state* A pointer to the fully populated 't_parse_state'
 * structure contains the parsed command list. Returns NULL if the input is
 * invalid or if a syntax or memory allocation failure occurs during parsing.
 */
t_parse_state	*parse_command(t_token *tokens)
{
	t_parse_state	*parser;

	parser = NULL;
	if (!tokens)
		return (printf("ERROR - Necesitamos Tokens\n"), NULL);
	parser = init_parser();
	if (!parser)
		return (free_tokens(&tokens), NULL);
	if (!handle_tokens(tokens, parser))
		return (NULL);
	if (parser->cmd_node)
		lstaddback_cmd(&parser->cmd_list, parser->cmd_node);
	return (parser);
}
