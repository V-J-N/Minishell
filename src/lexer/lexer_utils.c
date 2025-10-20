/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:03:18 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/20 20:25:13 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_utils.c
 * @brief Utility functions for memory management and list manipulation of
 * tokens.
 */
#include "minishell.h"

/**
 * @brief Frees all nodes and their contents in the token linked list.
 * It correctly deallocates the deep copies of the token values and the nodes
 * themselves.
 * @param tokens A double pointer to the head of the 't_token' list.
 * @note Correctly freeing both the internal string value and the node
 * structure is mandatory for a clean execution environment. The null check
 * at the begining ensures safe handling of empty or 'NULL' lists.
 */
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
		if(current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

/**
 * @brief Adds a new token node to the end of the token linked list.
 * This function handles the logic for appending a newly created token
 * to the existing list, maintaining the correct sequence of tokens
 * reads from the input.
 * @param head A pointer to a pointer of the first node in the list.
 * @param new_node A pointer to the new 't_token' node.
 * @note This function does not return a value. It is responsible for
 * manipulating pointers to links the nodes together.
 */
void	lstaddback_token(t_token **head, t_token *new_node)
{
	t_token	*current;

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
 * @brief Creates a new token for the lexer's linked list.
 * This function serves as the constructor for the 't_token' structure.
 * It handles memory allocation and initialization of all fields, including
 * the crucial deep copy of the token value.
 * @param value The string that represents the token's literal value.
 * @param type The enumerated type of the token.
 * @param quote The final quoting status of the token's value.
 * @param has_quotes A boolean indicating whether the accumulated word
 * originally contained any quotes.
 * @return t_token* A pointer to the newly created and intialized
 * 't_token' node.
 * Returns NULL if the input 'value' is 'NULL' or if memory allocation
 * fails at any point.
 */
t_token	*lstnew_token(char *value, t_token_type type,
	t_token_quote quote, bool has_quotes)
{
	t_token	*node;

	if (!value)
		return (NULL);
	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(value);
	if (!node->value)
		return (free(node), NULL);
	node->type = type;
	node->quote = quote;
	if (node->quote == SINGLE)
		node->is_expanded = false;
	else
		node->is_expanded = true;
	node->has_quotes = has_quotes;
	node->next = NULL;
	return (node);
}
