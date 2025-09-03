/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/03 14:38:07 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main function for parsing a command toke stream.
 */
#include "minishell.h"

/**
 * @brief 
 * 
 * @param block 
 * @param tokens 
 * @return true 
 * @return false 
 */
static bool	fill_word_node(t_command *node, t_token *tokens)
{
	t_token		*temp;
	size_t		i;

	temp = tokens;
	i = 0;
	while (temp && temp->type == WORD)
	{
		node->cmd_args[i] = ft_strdup(temp->value);
		if (!node->cmd_args[i])
			return (ft_free_array(node->cmd_args), false);
		temp = temp->next;
		i++;
	}
	node->type = CMD_SIMPLE;
	return (true);
}

/**
 * @brief 
 * 
 * @param tokens 
 * @return t_command* 
 */
static t_command	*word_node(t_token *tokens)
{
	t_token		*temp;
	t_command	*node;

	node = NULL;
	temp = tokens;
	node = ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->next = NULL;
	node->type = CMD_NONE;
	node->cmd_argc = lstsize_token_word(temp);
	node->cmd_args = ft_calloc(node->cmd_argc + 1, sizeof(char *));
	if (!node->cmd_args)
		return (free(node), NULL);
	return (node);
}

/**
 * @brief 
 * 
 * @param tokens 
 * @return t_command* 
 */
t_command	*parse_command(t_token *tokens)
{
	// t_command	*cmd_list;
	t_command	*cmd_node;
	t_token		*temp;
	t_redir		*redir_node;

	// cmd_list = NULL;
	cmd_node = NULL;
	redir_node = NULL;
	if (!tokens || tokens[0].type != WORD)
		return (printf("Lo primero necesita ser un comando\n"), NULL);
	temp = tokens;
	while (temp)
	{
		if (temp->type == WORD)
		{
			cmd_node = word_node(temp);
			if (!fill_word_node(cmd_node, temp))
				return (free(cmd_node), NULL);
			while (temp && temp->type == WORD)
				temp = temp->next;
			if (temp->type != WORD && temp->type != PIPE)
			{
				if (temp->next == NULL || temp->next->type != WORD)
					return (printf("se necesita un target"), free(cmd_node), NULL);
 				redir_node = create_redir(&cmd_node, temp);
				add_redir(&cmd_node, redir_node);
				//añadir node redir a cmd_list
				//avanzar dos posiciones redir + filename
				temp = temp->next->next;
			}
			// lstaddback_cmd(&cmd_list, cmd_node);
			// continue;
		}
		// else if (temp->type == PIPE)
		// {
		// 	// lógica pipes
		// }
	}
	return (cmd_node);
}
