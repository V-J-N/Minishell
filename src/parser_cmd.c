/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:05:12 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/09 14:05:28 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief 
 * 
 */

#include "minishell.h"

/**
 * @brief 
 * 
 * @param node 
 * @param tokens 
 * @return t_command* 
 */
void	add_args(t_command **node, t_token *tokens)
{
	t_arg	*temp;
	t_arg	*new_arg;

	(*node)->cmd_argc += 1;
	new_arg = ft_calloc(1, sizeof(t_arg));
	if (!new_arg)
		return ;
	new_arg->value = ft_strdup(tokens->value);
	new_arg->next = NULL;
	temp = (*node)->args;
	while (temp->next)
		temp = temp->next;
	temp->next = new_arg;
}

/**
 * @brief 
 * 
 * @param tokens 
 * @return t_command* 
 */
t_command	*update_empty_cmd(t_token *tokens, t_command **cmd_node)
{
	if (!*cmd_node)
		return (NULL);
	(*cmd_node)->args = ft_calloc(1, sizeof(t_arg));
	if (!(*cmd_node)->args)
		return (free_commands(cmd_node), NULL);
	(*cmd_node)->args->value = ft_strdup(tokens->value);
	if (!(*cmd_node)->args->value)
		return (free_commands(cmd_node), NULL);
	(*cmd_node)->args->next = NULL;
	(*cmd_node)->is_command = true;
	(*cmd_node)->cmd_argc = 1;
	return ((*cmd_node));
}

/**
 * @brief 
 * 
 * @param tokens 
 * @return t_command* 
 */
t_command	*create_empty_cmd()
{
	t_command	*node;

	node = NULL;
	node = ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->next = NULL;
	node->redirs = NULL;
	node->is_command = false;
	node->cmd_argc = 0;
	return (node);
}

/**
 * @brief 
 * 
 * @param tokens 
 * @return t_command* 
 */
t_command	*create_cmd(t_token *tokens)
{
	t_command	*node;

	node = NULL;
	node = ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->next = NULL;
	node->redirs = NULL;
	node->cmd_argc = 1;
	node->is_command = true;
	node->args = ft_calloc(1, sizeof(t_arg));
	if (!node->args)
		return (free(node), NULL);
	node->args->value = ft_strdup(tokens->value);
	if (!node->args->value)
		return (free(node->args), free(node), NULL);
	node->args->next = NULL;
	return (node);
}
