/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 11:55:04 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/28 11:56:11 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Search for the value in the list, given a key. Must be freed after use.
char	*get_value_by_key(t_env *env_list, const char *key)
{
	t_env	*temp;

	if (!env_list || !key)
		return (NULL);
	temp = env_list;
	while (temp)
	{
		if (ft_strlen(temp->key) == ft_strlen(key) &&
			ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	return (NULL);
}

/// @brief Returns the node matching the given key, or NULL if not found.
t_env	*find_node_by_key(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strlen(env_list->key) == ft_strlen(key) &&
			ft_strncmp(env_list->key, key, ft_strlen(key)) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static void	delete_free(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env->full_env);
	free(env);
	return ;
}

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
		if (ft_strlen(curr->key) == ft_strlen(key) &&
		ft_strncmp(curr->key, key, ft_strlen(key)) == 0)
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
