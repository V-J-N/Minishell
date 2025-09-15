/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/15 11:37:10 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main function for parsing a command token stream.
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

static bool	handle_tokens(t_token *tokens, t_parse_state *p_struct)
{
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if (!handle_word(tokens, p_struct))
				return (free_tokens(&tokens), parse_error("minishell: WORD parse error", p_struct), NULL);
			tokens = tokens->next;
		}
		else if (tokens->type == PIPE)
		{
			if (!handle_pipe(tokens, p_struct))
				return (free_tokens(&tokens), parse_error("minishell: PIPE parse error", p_struct), NULL);
			tokens = tokens->next;
		}
		else if (is_redir(tokens))
		{
			if (!handle_redir(tokens, p_struct))
				return (free_tokens(&tokens), parse_error("minishell: REDIR parse error", p_struct), NULL);
			tokens = tokens->next->next;
		}
	}
	return (true);
}

/**
 * @brief Parses a token stream to create a command block list.
 * This function orchestates the entire parsing process. It initializes a
 * 't_parse_state' structure to manage the parsing state, the iterates through
 * the token stream to identify and process different token types.
 * @param tokens A pointer to the head of the 't_token' linked list, which
 * represents the command line to be parsed.
 * @return t_parse_state* A pointer to the fully populated 't_parse_state'
 * structure, which contains the parsed command list. Returns 'NULL' if the
 * input is invalid or if a syntax or memory allocation failure occurs.
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
