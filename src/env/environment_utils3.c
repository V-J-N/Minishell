/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 11:55:04 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/20 17:36:54 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file environment_utils3.c
 * @brief Environment list searching and deletion utilities.
 */
#include "minishell.h"

/**
 * @brief Searches the environment list for a specific key and returns a deep
 * copy of its value.
 * This function traverses the 't_env' list to find a node whose 'key' field
 * matches the input 'key'. It performs a rigorous comparison: checking both
 * the length and the content using 'ft_strncmp'
 * @param env_list The head of the 't_env' linked list.
 * @param key The environment variable key to search for.
 * @return A new dynamically allocated string containing the variable's value.
 * Returns NULL if the key is not found or if inputs are invalid.
 */
char	*get_value_by_key(t_env *env_list, const char *key)
{
	t_env	*temp;

	if (!env_list || !key)
		return (NULL);
	temp = env_list;
	while (temp)
	{
		if (ft_strlen(temp->key) == ft_strlen(key)
			&& ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	return (ft_strdup(""));
}

/**
 * @brief Searches the environment list for a key and returns the pointer
 * to its node.
 * @param env_list The head of the 't_env' linked list.
 * @param key The environment variable key to search for.
 * @return t_env* A pointer to the matching 't_env' node. Returns NULL if
 * the key is not found.
 */
t_env	*find_node_by_key(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strlen(env_list->key) == ft_strlen(key)
			&& ft_strncmp(env_list->key, key, ft_strlen(key)) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

/**
 * @brief Internal helper function to safety deallocate a single environment
 * node and its contents.
 * This routine encapsulates the deep freeing logic for the one 't_env' node,
 * ensuring all three internal strings are freed before the node structure
 * itself is released.
 * @param env The 't_env' node to be deallocated.
 */
static void	delete_free(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env->full_env);
	free(env);
	return ;
}

/**
 * @brief Removes and safely deallocates the environment variable node matching
 * the given key.
 * This function implements the core logic for the 'unset' builtin. It traverses
 * the list using pointers for the currnet node and the preceding node to
 * safely unlink the target node.
 * @param env A double pointer to the head of the 't_env' list, allowing the
 * function to modify the head if necessary.
 * @param key The environment variable key to be deleted.
 */
void	delete_env_key(t_env **env, const char *key)
{
	t_env	*prev;
	t_env	*curr;

	if (!env || !*env || !key)
		return ;
	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (ft_strlen(curr->key) == ft_strlen(key)
			&& ft_strncmp(curr->key, key, ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			delete_free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	return ;
}
