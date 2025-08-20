/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_memory.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:37:53 by serjimen          #+#    #+#             */
/*   Updated: 2025/08/20 02:35:14 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Deallocates all nodes and their contents in the environment
 * linked list.
 * This function iterates through the entire 't_env' linked list, starting from
 * the head. For each node, it performs a full cleanup by first deallocating the
 * memory for the three strings it contains ('key', 'value', 'full_env'), and then
 * freeing the memory for the node itself.
 * @param env A double pointer to the head of the 't_env' list.
 * @note The use of a temporary 'next' pointer is a standard and essential
 * pattern for safely traversing and freeing a linked list, as it prevents
 * losing the reference to the next node before freeing the current one.
 */
void	free_environment(t_env **env)
{
	t_env	*current;
	t_env	*next;

	if (!env || !*env)
		return ;
	current = *env;
	while (current != NULL)
	{
		next = current->next;
		free(current->full_env);
		free(current->value);
		free(current->key);
		free(current);
		current = next;
	}
	*env = NULL;
}
