/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:19:48 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/31 12:43:06 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser_redir.c
 * @brief Utility functions for managing redirections in the parser.
 */
#include "minishell.h"

/**
 * 
 */
static void	init_redir(t_redir *redir)
{
	redir->exp_file = NULL;
	redir->env_file = NULL;
	redir->next = NULL;
	redir->heredoc_fd = -1;
	redir->i = 0;
}

/**
 * @brief Appends a new redirection node to a command's redirection list.
 * This function adds a new redirection node 't_redir' to an existing
 * 't_command' node's redirection list.
 * @param cmd_node A pointer to the 't_command' node.
 * @param new_redir A pointer to the 't_redir' node.
 */
void	add_redir(t_command *cmd_node, t_redir *new_redir)
{
	t_redir	*current;

	if (!cmd_node || !new_redir)
		return ;
	if (!cmd_node->redirs)
	{
		cmd_node->redirs = new_redir;
		return ;
	}
	current = cmd_node->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/**
 * @brief Creates a new redirection node from a token stream.
 * This function is a constructor for a 't_redir' node. It allocates and
 * initializes a new node with the redirection type and the corresponding
 * file name.
 * @param tokens A pointer to the token representing the redirection symbol.
 * @return t_redir* A pointer to the newly created 't_redir' node. Returns
 * NULL if memory allocation fails.
 */
t_redir	*create_redir(t_token *tokens)
{
	t_token		*temp;
	t_redir		*redir;

	redir = NULL;
	temp = tokens;
	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = tokens->type;
	redir->quote = temp->next->quote;
	redir->has_quotes = temp->next->has_quotes;
	redir->is_expanded = temp->next->is_expanded;
	redir->file = ft_strdup(temp->next->value);
	if (!redir->file)
		return (free_redirs(redir), NULL);
	if (redir->type == HEREDOC && contains_dollar(redir->file))
		redir->is_expanded = false;
	if (redir->type == HEREDOC && !contains_dollar(redir->file)
		&& redir->quote == DOUBLE)
		redir->is_expanded = false;
	init_redir(redir);
	return (redir);
}
