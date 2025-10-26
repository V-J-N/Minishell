/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:36:51 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/24 20:52:21 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file environment_utils.c
 * @brief Utilities for environment list manipulation and variable parsing.
 * This file provides functions to build, navigatem and populate the internal
 * envinronment linked list.
 */
#include "minishell.h"

/**
 * @brief Adds a new node to the end of the environment linked list.
 * This function appends a given 't_env' node to the end of the linked list
 * pointed to by 'head'. It correctly handles two main cases: and empty list,
 * where the new node becomes the head, and a non-empty list, where it
 * traverses to the last node and links the new node there.
 * @param head A pointer to pointer to the head f the 't_env' list.
 * @param new_node A pointer to the new 't_env' node that is to be added to
 * the list.
 * @note This function does not allocate or free memory, its only manipulates
 * pointers. It is assumed that 'new_node' has already been allocated and
 * correctly initialized by the caller.
 */
void	ft_addback_mini_env(t_env **head, t_env *new_node)
{
	t_env	*current;

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

/**
 * @brief Helper function for ft_lstnewmini_env to perform deep string copies.
 * This auxiliary function handles the dynamic memory allocation and deep
 * copying for the three strings. It includes detailed error handling to free
 * memory allocated in previous steps if any subsequent allocation fails.
 * @param key The key string to copy.
 * @param value The value string to copy.
 * @param full The full "KEY=value" string to copy.
 * @param node The already allocated t_env structure.
 * @return t_env* The populated node on success, or NULL on memory
 * allocation failure.
 */
static t_env	*aux_lstnew(char *key, char *value, char *full, t_env *node)
{
	if (key)
	{
		node->key = ft_strdup(key);
		if (!node->key)
			return (free(node), NULL);
	}
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (free(node->key), free(node), NULL);
	}
	if (full)
	{
		node->full_env = ft_strdup(full);
		if (!node->full_env)
			return (free(node->key), free(node->value), \
			free(node), NULL);
	}
	return (node);
}

/**
 * @brief Creates a new node for the environment linked list.
 * This function is the constructor for the 't_env' node. It allocates
 * memory for the node structure and performs a deep copy of the 'key',
 * 'value', and 'full' strings using 'ft_strdup'.
 * @param key The key (name) of the environment variable.
 * @param value The value of the environment variable.
 * @param full The full "KEY=value" string.
 * @return A pointer to the newly created 't_env' node.
 * Returns NULL if memory allocation fails for the node or any of
 * its internal strings.
 * @note The deep copy ensures that the data within the list is
 * self-contained and independent of the temporary strings used during
 * environment parsing.
 */
t_env	*ft_lstnew_mini_env(char *key, char *value, char *full)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->full_env = NULL;
	new_node->next = NULL;
	new_node = aux_lstnew(key, value, full, new_node);
	if (!new_node)
		return (NULL);
	return (new_node);
}

/**
 * @brief Extracts and allocates a deep copy of the key from an environment
 * string.
 * This function parses an environtment variable string in the format
 * 'KEY=value' and dynamically allocates memory for the 'KEY' part. It iterates
 * through the string until it finds the '=' character, calculates the required
 * memory, and then copies the key. The function uses 'ft_calloc' to ensure the
 * allocated memory is initialized to zero, automatically providing a null
 * terminator at the end of the string.
 * @param str The source string containing the environment variable.
 * @return A new dynamically allocated string containing the extracted key.
 * Returns NULL if memory allocation fails.
 */
char	*set_key(char *str)
{
	int		i;
	char	*key;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_calloc(i + 1, sizeof(char));
	if (!key)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		key[i] = str[i];
		i++;
	}
	return (key);
}

/**
 * @brief Extracts and allocates a deep copy of the value from an environment
 * string.
 * This function parses an environment variable string in the format
 * 'KEY=value' and dynamically allocates memory for the 'value' part. It first
 * find the position of the '=' character to determine the start of the value.
 * It then uses 'ft_calloc' to allocate the necessary memory, ensuring the new
 * string is null-terminated. Finally, it copies the value from the source
 * string to the new memory block.
 * @param str The source string containing the environment variable.
 * @return A new dynamically allocated string containing the extracted value.
 * Returns NULL if memory allocation fails.
 */
char	*set_value(char *str)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	j = 0;
	while (str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	value = ft_calloc(ft_strlen(str) - i + 1, sizeof(char));
	if (!value)
		return (NULL);
	while (str[i] != '\0')
	{
		value[j] = str[i];
		j++;
		i++;
	}
	return (value);
}
