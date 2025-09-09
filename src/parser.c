/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/09 14:17:00 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief Main function for parsing a command token stream.
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
/* static bool	fill_word_node(t_command *node, t_token *tokens)
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
	return (true);
} */

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
t_command	*parse_command(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_redir		*redir_node;
	t_token		*temp;

	cmd_list = NULL;
	current_cmd = NULL;
	redir_node = NULL;
	if (!tokens)
		return (printf("ERROR - Necesitamos Tokens\n"), NULL);
	temp = tokens;
	while (temp)
	{
		if (temp->type == PIPE)
			return (printf("ERROR - Lo primero no puede ser un PIPE\n"), NULL);
		// Mas adelante se gestionaran los pipes, si lo primero es un pipe falla
		// Si el pipe esta entremedias se crea el nodo y pasamos a un nodo nuevo.
		else if (temp->type == WORD)
		{
			if (!current_cmd)
				current_cmd = create_cmd(temp);
			else if (!current_cmd->is_command)
				current_cmd = update_empty_cmd(temp, &current_cmd);
			else
				add_args(&current_cmd, temp);
			temp = temp->next;
		}
		else if (is_redir(temp))
		{
			if (temp->next == NULL || temp->next->type != WORD)
				return (printf("ERROR - Se necesita un target file\n"), free_commands(&current_cmd), NULL);
			if (!current_cmd)
				current_cmd = create_empty_cmd();
			redir_node = create_redir(temp);
			add_redir(&current_cmd, redir_node);
			temp = temp->next->next;
		}
		else
			return (printf("ERROR Desconocido"), NULL);
	}
	if (current_cmd)
		lstaddback_cmd(&cmd_list, current_cmd);
	return (cmd_list);
}
