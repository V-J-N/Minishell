/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:37:53 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/24 21:07:05 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file environment_utils2.c
 * @brief Environment list cleanup and array conversion utilities.
 */
#include "minishell.h"

/**
 * @brief Deallocates all nodes and their contents in the environment
 * linked list.
 * This function iterates through the entire 't_env' linked list, starting from
 * the head. For each node, it performs a full cleanup by first deallocating
 * the memory for the three strings it contains ('key', 'value', 'full_env')
 * and then freeing the memory for the node itself.
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

/**
 * @brief Searches for a specific environment variable by key and returns a
 * deep copy of the full "KEY=value" string.
 * This function is a utility for querying the environment list. It traverses
 * the list and compares the 'key' field of each node with the target key.
 * @param env_list The head of the 't_env' linked list.
 * @param key The environment variable name (key) to search for.
 * @return A new dynamically allocated string containing the full "KEY=value"
 * format. Returns NULL if the key is not found iun the list.
 */
char	*get_full_env(t_env *env_list, const char *key)
{
	char	*full_env;

	if (!key)
		return (NULL);
	while (env_list)
	{
		if ((ft_strlen(env_list->key) == ft_strlen(key)) && \
		(ft_strncmp(env_list->key, key, ft_strlen(key)) == 0))
		{
			full_env = ft_strdup(env_list->full_env);
			return (full_env);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

/**
 * @brief Calculates the number of nodes in the environment linked list.
 * @param lst A pointer to the head of the 't_env' list.
 * @return int The number of nodes in the list.
 */
static int	ft_envlst_size(t_env *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}

/**
 * @brief Converts the internal 't_env' linked list into a dynamicaly allocated
 * NULL-terminated array of "KEY=value" strings.
 * This function transforms the structured environment data back into the
 * standard 'char **' array format required by the 'execve' system call
 * for the executing external programs.
 * @param envlist A pointer to a pointer to the head of the environment list.
 * @return char** Returns a dynamically allocated, NULL-terminated array of
 * environment strings. Returns NULL if memory allocation fails.
 */
char	**envlist_to_arr(t_env **envlist)
{
	int		count;
	char	**envarr;
	t_env	*temp;
	int		i;

	count = ft_envlst_size(*envlist);
	envarr = ft_calloc(count + 1, sizeof(char *));
	if (!envarr)
		return (NULL);
	temp = *envlist;
	i = 0;
	while (temp)
	{
		envarr[i] = ft_strdup(temp->full_env);
		if (!envarr[i])
			return (ft_free_array(envarr), NULL);
		temp = temp->next;
		i++;
	}
	envarr[i] = NULL;
	return (envarr);
}
