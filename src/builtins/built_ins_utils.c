/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:34:38 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 12:47:48 by vjan-nie         ###   ########.fr       */
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

bool	is_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5) ||
			!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4) ||
			!ft_strncmp(cmd, "env", 4));
}

bool	is_parent_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5));
}

char	**args_to_array(t_arg *args)
{
	size_t	count = 0;
	t_arg	*tmp = args;
	char	**arr;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = ft_calloc(count + 1, sizeof(char *));
	if (!arr)
		return NULL;
	tmp = args;
	for (size_t i = 0; i < count; i++)
	{
		arr[i] = ft_strdup(tmp->value);
		if (!arr[i])
			return (ft_free_array(arr), NULL);
		tmp = tmp->next;
	}
	return arr;
}

/* void	update_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*node;
	char	*new_full;

	node = find_node_by_key(*env, key);
	new_full = ft_strjoin(key, "=");
	new_full = ft_strjoin_free(new_full, value); // une key= + value y libera

	if (node)
	{
		free(node->value);
		free(node->full_env);
		node->value = ft_strdup(value);
		node->full_env = ft_strdup(new_full);
	}
	else
	{
		t_env *new_node = ft_lstnew_mini_env(ft_strdup(key), ft_strdup(value), ft_strdup(new_full));
		ft_addback_mini_env(env, new_node);
	}
	free(new_full);
}

char *ft_strjoin_free(char *s1, const char *s2)
{
	char *joined;
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
} */
