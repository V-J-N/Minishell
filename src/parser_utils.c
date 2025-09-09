/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:42:15 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/09 17:24:21 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		temp = command_list->redirs->next;
		count ++;
	}
	return (count);
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
//		ft_free_array(current->cmd_args);
		free(current);
		current = next;
	}
	*commands = NULL;
}

/**
 * @brief 
 * 
 * @param lst 
 * @return size_t 
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

/**
 * @brief 
 * 
 * @param ptr 
 * @param size 
 * @return void* 
 */
/*void	*ft_realloc(void *ptr, size_t size, size_t oldsize)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
		return (free(ptr), NULL);
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (oldsize < size)
		copy_size = oldsize;
	else
		copy_size = size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
} */