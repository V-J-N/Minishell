/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:05:12 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/21 13:05:57 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser_cmd.c
 * @brief Utility functions for command block management.
 */

#include "minishell.h"

/**
 * @brief Appends a new argument to a command's argument list.
 * This function adds a new argument to an existing 't_command' node.
 * @param tokens A pointer to the 't_token' node.
 * @param cmd_node A pointer to the 't_command' node.
 * @return true on success.
 * @return false if memory allocation for the new argument ir its value fails.
 * @note This function assumes the 'cmd_node' is already initialized and its
 * 'args' list has at least one element. It is crucial for handling commands
 * with multiple arguments.
 */
bool	add_args(t_token *tokens, t_command *cmd_node)
{
	t_arg	*temp;
	t_arg	*new_arg;

	new_arg = ft_calloc(1, sizeof(t_arg));
	if (!new_arg)
		return (false);
	new_arg->value = ft_strdup(tokens->value);
	if (!new_arg->value)
		return (false);
	new_arg->quote = tokens->quote;
	new_arg->has_quotes = tokens->has_quotes;
	new_arg->is_expanded = tokens->is_expanded;
	new_arg->i = 0;
	new_arg->exp_value = NULL;
	new_arg->env_value = NULL;
	new_arg->next = NULL;
	temp = cmd_node->args;
	while (temp->next)
		temp = temp->next;
	temp->next = new_arg;
	cmd_node->cmd_argc += 1;
	return (true);
}

/**
 * @brief Updates an empty command node with a command name.
 * This function takes and existing 't_command' node that was created
 * in an empty state and populates it with its primary command name.
 * @param tokens A pointer to the 't_token' node.
 * @param cmd_node A pointer to the 't_command' node to be updated.
 * @return true on success.
 * @return false if the 'cmd_node' is invalid or if a memory allocation fails.
 */
bool	update_empty_cmd(t_token *tokens, t_command *cmd_node)
{
	if (!cmd_node || cmd_node->args != NULL)
		return (false);
	cmd_node->args = ft_calloc(1, sizeof(t_arg));
	if (!cmd_node->args)
		return (false);
	cmd_node->args->value = ft_strdup(tokens->value);
	if (!cmd_node->args->value)
		return (false);
	cmd_node->args->quote = tokens->quote;
	cmd_node->args->has_quotes = tokens->has_quotes;
	cmd_node->args->is_expanded = tokens->is_expanded;
	cmd_node->args->i = 0;
	cmd_node->args->next = NULL;
	cmd_node->is_command = true;
	cmd_node->cmd_argc = 1;
	return (true);
}

/**
 * @brief Creates a new command block without an initial command name.
 * This function serves as a constructor for a 't_command' node when a
 * redirection is encountered before a command name.
 * A key feature of this function is setting the 'is_command' field to 'false'
 * and 'cmd_argc' to 0. This state signals to the parser that while a command
 * block exits, it is still awaiting its primary command name.
 * @return t_command* A pointer to the newly created and empty 't_command' node.
 * Returns NULL if memory allocation fails.
 */
t_command	*create_empty_cmd(void)
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
 * @brief Creates a new command block with a command name.
 * This function serves as the constructor for a 't_command' node.
 * @param tokens A pointer to the token representing the command name.
 * @return t_command* A pointer to the newly created 't_command' node.
 * Returns NULL if memory allocation fails at any point.
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
		return (free_commands(&node), NULL);
	node->args->value = ft_strdup(tokens->value);
	if (!node->args->value)
		return (free_commands(&node), NULL);
	node->args->quote = tokens->quote;
	node->args->has_quotes = tokens->has_quotes;
	node->args->is_expanded = tokens->is_expanded;
	node->args->exp_value = NULL;
	node->args->env_value = NULL;
	node->args->i = 0;
	node->args->next = NULL;
	return (node);
}
