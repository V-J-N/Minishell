/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:33:51 by serjimen          #+#    #+#             */
/*   Updated: 2025/08/21 09:50:09 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <structs.h>

/* static t_token	*ft_lstnew_mini(char *value, t_token_type type)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->next = NULL;
	return (node);
} */

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
/* static void	ft_ps_addback(t_token **head, t_token *new_node)
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
} */

/* t_token	*create_stack(char **array)
{
	t_token	*new_node;
	t_token	*stack;
	size_t	i;

	stack = NULL;
	i = 0;
	while (array[i])
	{
		new_node = ft_ps_newint();
		if (!new_node)
		{
			free_stack(&stack);
			free_array_c(array);
			return (NULL);
		}
		ft_ps_addback(&stack, new_node);
		i++;
	}
	return (stack);
} */

char	**ft_tokenizer(char *str)
{
	char	**tokens;
	int		i;

	tokens = ft_split(str, ' ');
	i = 0;
	while (tokens[i])
	{
		printf("Token %d: [%s]\n", i, tokens[i]);
		i++;
	}
	return (tokens);
}