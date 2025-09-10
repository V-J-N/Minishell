/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/10 18:02:39 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main function for parsing a command token stream.
 */
#include "minishell.h"

static bool	handle_redir(t_token *tokens, t_parse_state *p_struct)
{
	if (tokens->next == NULL || tokens->next->type != WORD)
	{
		free_tokens(&tokens);
		return (false);
	}
	if (!p_struct->cmd_node)
		p_struct->cmd_node = create_empty_cmd();
	p_struct->redir_node = create_redir(tokens);
	add_redir(p_struct->cmd_node, p_struct->redir_node);
	return (true);
}

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
 * This function iterates throught a token stream, identifying a 't_command'
 * linked list.
 * @param tokens A pointer to the head of the 't_token' linked list, which
 * represents the command line to be parsed.
 * @return t_command* A pointer to the head of the newly created 't_command'
 * list. Returns NULL if the input is invalid or if a syntax or memory
 * allocation failure occurs.
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
		return (NULL);
	temp = tokens;
	while (temp)
	{
		if (temp->type == PIPE)
			return (parse_error("minishell: parse error", parser), NULL);
		else if (temp->type == WORD)
		{
			if (!handle_word(temp, parser))
				return (parse_error("minishell: parse error", parser), NULL);
			temp = temp->next;
		}
		else if (is_redir(temp))
		{
			if (!handle_redir(temp, parser))
				return (parse_error("minishell: parse error", parser), NULL);
			temp = temp->next->next;
		}
		else
			return (parse_error("minishell: parse error", parser), NULL);
	}
/* 	if (current_cmd)
		lstaddback_cmd(&cmd_list, current_cmd);*/
	return (parser);
}
