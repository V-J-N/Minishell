/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:42:15 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/09 12:34:48 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(t_token *list)
{
	if (list->type == REDIR_IN || list->type == REDIR_OUT ||
				list->type == APPEND || list->type == HEREDOC)
		return (true);
	return (false);
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