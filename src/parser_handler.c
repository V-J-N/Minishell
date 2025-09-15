/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:33:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/15 11:34:18 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_pipe(t_token *tokens, t_parse_state *p_struct)
{
	if (!p_struct->cmd_node)
		return (false);
	if (!tokens->next || tokens->next->type != WORD)
		return (false);
	lstaddback_cmd(&p_struct->cmd_list, p_struct->cmd_node);
	p_struct->cmd_node = NULL;
	return (true);
}

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
bool	handle_redir(t_token *tokens, t_parse_state *p_struct)
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
bool	handle_word(t_token *tokens, t_parse_state *p_struct)
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
