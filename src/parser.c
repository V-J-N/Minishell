/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/12 12:02:34 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main function for parsing a command token stream.
 */
#include "minishell.h"

/**
 * @brief Processes and incorporates a redirection token into the current
 * command block.
 * @param tokens A pointer to the current 't_token' node.
 * @param p_struct A pointer to the 't_parse_state' structure.
 * @return true on success, indicating that the redirection was processed
 * and the state was updated successfully.
 * @return false on a syntax error or a memory allocation failure.
 * @note This function correctly validates the redirection syntax before
 * processing the tokens.
 */
static bool	handle_redir(t_token *tokens, t_parse_state *p_struct)
{
	if (tokens->next == NULL || tokens->next->type != WORD)
		return (false);
	if (!p_struct->cmd_node)
		p_struct->cmd_node = create_empty_cmd();
	p_struct->redir_node = create_redir(tokens);
	add_redir(p_struct->cmd_node, p_struct->redir_node);
	return (true);
}

/**
 * @brief Processes and incorporates a WORD token into the current command
 * block.
 * @param tokens A pointer to the current 't_token' node to be processed.
 * @param p_struct A pointer to the 't_parse_state' structure that holds the
 * parser's current state.
 * @return true on success, indicating that the token was processed and the
 * state was updated successfully.
 * @return false on failure, typically due to a memory allocation error, after
 * which the caller should perform necessary cleanup.
 * @note This function correctly distinguishes between a command name and its
 * arguments.
 */
static bool	handle_word(t_token *tokens, t_parse_state *p_struct)
{
	if (!p_struct->cmd_node)
		p_struct->cmd_node = create_cmd(tokens);
	else if (!p_struct->cmd_node->is_command)
	{
		if (!(update_empty_cmd(tokens, p_struct->cmd_node)))
		{
			free_tokens(&tokens);
			return (false);
		}
	}
	else
	{
		if (!(add_args(tokens, p_struct->cmd_node)))
		{
			free_tokens(&tokens);
			return (false);
		}
	}
	return (true);
}

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
	t_token			*temp;

	parser = NULL;
	if (!tokens)
		return (printf("ERROR - Necesitamos Tokens\n"), NULL);
	parser = init_parser();
	if (!parser)
		return (free_tokens(&tokens), NULL);
	temp = tokens;
	while (temp)
	{
		/*
		if (temp->type == PIPE)
			return (free_tokens(&tokens), parse_error("minishell: PIPE parse error", parser), NULL);
			*/
		if (temp->type == WORD)
		{
			if (!handle_word(temp, parser))
				return (free_tokens(&tokens), parse_error("minishell: WORD parse error", parser), NULL);
			temp = temp->next;
		}
		else if (temp->type == PIPE)// sugerencia de la IA, lo uso para test
		{
			if (!parser->cmd_node)
				return (free_tokens(&tokens), parse_error("minishell: pipe sin comando previo", parser), NULL);
			lstaddback_cmd(&parser->cmd_list, parser->cmd_node);
			parser->cmd_node = NULL;
			temp = temp->next;
		}
		else if (is_redir(temp))
		{
			if (!handle_redir(temp, parser))
				return (free_tokens(&tokens), parse_error("minishell: REDIR parse error", parser), NULL);
			temp = temp->next->next;
		}
	}
	if (parser->cmd_node)
		lstaddback_cmd(&parser->cmd_list, parser->cmd_node);
	return (parser);
}
