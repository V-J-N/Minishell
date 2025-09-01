/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:42:15 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/01 17:30:25 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ft_free_array(current->cmd_args);
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
