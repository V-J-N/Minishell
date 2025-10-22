/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:38:53 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/22 19:44:03 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser_free.c
 * @brief Utility functions for memory management in the parser.
 */
#include "minishell.h"

/**
 * @brief Frees all nodes and their contents in a redirection linked list.
 * @param redirs A pointer to the head of the 't_redir' list to be freed.
 */
void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	if (!redirs)
		return ;
	current = redirs;
	while (current != NULL)
	{
		next = current->next;
		if (current->type == HEREDOC && current->heredoc_fd > 2)
			close(current->heredoc_fd);
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

/**
 * @brief Frees all nodes and their contents in an argument linked list.
 * @param args A pointer to the head of the 't_arg' list to be freed.
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
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

/**
 * @brief Frees all command nodes and their contents in a command linked list.
 * @param commands A double pointer to the head of the 't_command' list.
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
		if (current->args)
			free_args(current->args);
		if (current->redirs)
			free_redirs(current->redirs);
		free(current);
		current = next;
	}
	*commands = NULL;
}

/**
 * @brief Frees the entire parser state structure and its contents.
 * @param parse_struct A pointer to a pointer to the 't_parse_state'.
 * @note It is designed to be called both upon sucessful parsing and in
 * case of a fatal error, guaranteeing a clean exit without memory leaks.
 */
void	free_parser(t_parse_state **parse_struct)
{
	if (!parse_struct || !*parse_struct)
		return ;
	if ((*parse_struct)->cmd_list)
		free_commands(&(*parse_struct)->cmd_list);
	else if ((*parse_struct)->cmd_node)
		free_commands(&(*parse_struct)->cmd_node);
	free(*parse_struct);
	*parse_struct = NULL;
}
