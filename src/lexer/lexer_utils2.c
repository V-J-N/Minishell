/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:10:15 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 17:36:34 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_utils2.c
 * @brief Utility functions for handling token lists as arrays and memory
 * management.
 */
#include "minishell.h"

/**
 * @brief Calculates the number of nodes in a token linked list.
 * @param lst A pointer to the head of the 't_token' list.
 * @return size_t The number of nodes in the list.
 */
static size_t	lstsize_token(t_token *lst)
{
	size_t	counter;

	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}

/**
 * @brief Converts a token linked list into a dynamically allocated array
 * of strings.
 * This function creates a deep copy of the token values from a linked list.
 * @param tokenlist The head of the 't_token' linked list to be converted.
 * @return char** Return a new dynamically allocated, NULL-terminated array
 * of strings. Returns NULL if memory allocation fails at any point.
 * @note The use of 'ft_strdup' is a critical design choice here. It prevents
 * the new array from holding pointers to the original token list's memory,
 * which would lead to dangling pointers if the list were to be freed before
 * the array.
 */
char	**tokenlist_to_arr(t_token *tokenlist)
{
	t_token	*temp;
	char	**tokenarr;
	size_t	count;
	size_t	i;

	count = lstsize_token(tokenlist);
	tokenarr = ft_calloc(count + 1, sizeof(char *));
	if (!tokenarr)
		return (NULL);
	temp = tokenlist;
	i = 0;
	while (temp)
	{
		tokenarr[i] = ft_strdup(temp->value);
		if (!tokenarr[i])
			return (ft_free_array(tokenarr), NULL);
		temp = temp->next;
		i++;
	}
	tokenarr[i] = NULL;
	return (tokenarr);
}

/**
 * @brief Custom implementation of memory reallocation for string buffers.
 * This function handles the dynamic resizing of the lexer's word accumulation
 * buffer. It is called by 'tokenize_char' when the buffer capacity is reached.
 * @param buffer The pointrer to the original memory block to be resized.
 * @param capacity The new, larger size (in bytes) for the memory block.
 * @return char* A pointer to the newly allocated and copied memory block.
 * Returns NULL if memory allocation fails.
 */

char	*ft_realloc(char *buffer, size_t capacity)
{
	char	*new_buffer;
	size_t	i;

	new_buffer = ft_calloc(capacity, sizeof(char));
	if (!new_buffer)
		return (NULL);
	i = 0;
	while (buffer[i])
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	new_buffer[i] = '\0';
	free(buffer);
	return (new_buffer);
}
