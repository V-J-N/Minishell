/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:03:18 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/08/27 11:05:51 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

t_token	*ft_lstnew_token(char *value, t_token_type type)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(value);
	node->type = type;
	node->next = NULL;
	return (node);
}

/**
 * @brief Appends the node to the en of the stack.
 * This function adds the given node to the end of the linked list representing
 * the stack.
 * If the stack is empty, the new node becomes the head of the stack.
 * @param head A pointer to the pointer of the first node in the stack.
 * @param new_node The node to append to the stack.
 * @note This function does not return a value. It modifies the stack in place.
 * @warning If either head or new_node is NULL, the function does nothing.
 */
void	ft_addback_token(t_token **head, t_token *new_node)
{
	t_token	*current;

	if (!head || !new_node)
		return ;
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