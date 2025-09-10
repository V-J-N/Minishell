/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:38:53 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/10 12:38:16 by sergio-jime      ###   ########.fr       */
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
 * @param redirs
 */
static void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	if (!redirs)
		return ;
	current = redirs;
	while (current != NULL)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}

/**
 * @brief 
 * 
 * @param args 
 */
static void	free_args(t_arg *args)
{
	t_arg	*current;
	t_arg	*next;

	if (!args)
		return ;
	current = args;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

/**
 * @brief 
 * 
 * @param commands 
 */
void	free_commands(t_command **commands)
{
	t_command	*current;
	t_command	*next;

	if (!commands || !*commands)
		return ;
	current = *commands;
	while (current != NULL)
	{
		next = current->next;
		free_args(current->args);
		free_redirs(current->redirs);
		free(current);
		current = next;
	}
	*commands = NULL;
}

void	free_parser(t_parse_state **parse_struct)
{
	if (!parse_struct || !*parse_struct)
		return ;
	if ((*parse_struct)->cmd_list)
		free_commands(&(*parse_struct)->cmd_list);
	if ((*parse_struct)->cmd_node)
		free_commands(&(*parse_struct)->cmd_node);
	free(*parse_struct);
	*parse_struct = NULL;
}