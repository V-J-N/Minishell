/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:42:15 by sergio-jime       #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/11 13:12:21 by sergio-jime      ###   ########.fr       */
=======
/*   Updated: 2025/09/10 09:39:08 by vjan-nie         ###   ########.fr       */
>>>>>>> 5cad140a46a1768d85fc1df4c96bb4e5922f7e82
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser_utils.c
 * @brief 
 */
#include "minishell.h"

size_t	number_of_commands(t_command *command_list)
{
	t_command	*temp;
	size_t		count;

	if (!command_list)
		return (0);
	temp = command_list;
	count = 0;
	while (temp)
	{
		temp = temp->next;
		count ++;
	}
	return (count);
}

size_t	number_of_redirs(t_command *command_list)
{
	size_t	count;
	t_redir	*temp;

	if (!command_list)
		return (0);
	if (!command_list->redirs)
		return (0);
	temp = command_list->redirs;
	count = 0;
	while(temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

/**
 * @brief Appends a new command node 't_command' to the end of the command
 * list pointed to by 'head'.
 * @param head A pointer to a pointer to the head of the 't_command' list.
 * @param new_node A pointer to the new 't_command' node.
 */
void	lstaddback_cmd(t_command **head, t_command *new_node)
{
	t_command	*current;

	if (!head || !new_node)
		return ;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
}

/**
 * @brief Check if a token's type corresponds to a redirection.
 * @param list A pointer to the 't_token' node.
 * @return true if the token is a redirection type.
 * @return false otherwise.
 */
bool	is_redir(t_token *list)
{
	if (list->type == REDIR_IN || list->type == REDIR_OUT
		|| list->type == APPEND || list->type == HEREDOC)
		return (true);
	return (false);
}

/**
 * @brief Utility function to count consecutive WORD tokens.
 * @param lst A pointer to the 't_token' node.
 * @return size_t The number of consecutive WORD tokens found.
 */
size_t	lstsize_token_word(t_token *lst)
{
	size_t	counter;

	counter = 0;
	while (lst && lst->type == WORD)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}
