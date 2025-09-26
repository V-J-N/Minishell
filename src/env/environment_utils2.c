/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:37:53 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/24 12:22:47 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/// @brief Search for the 'full_env' in the list, given a key
char	*get_full_env(t_env *env_list, const char *key)
{
	char	*full_env;

	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, ft_strlen(key)) == 0)
		{
			full_env = ft_strdup(env_list->full_env);
			return (full_env);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

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
 * @brief Extracts the data from our
 * environment data structure list and returns
 * an array of strings that can be used by execve()
 * */
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
