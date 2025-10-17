/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:08:49 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/17 14:11:58 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_assignation(char *key, char *new_v, t_env **env, char *full_var)
{
	t_env	*node;
	t_env	*new_node;

	node = find_node_by_key(*env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(new_v);
		free(node->full_env);
		node->full_env = ft_strdup(full_var);
	}
	else
	{
		new_node = ft_lstnew_mini_env(key, new_v, full_var);
		if (new_node)
			ft_addback_mini_env(env, new_node);
	}
	return ;
}

int	ft_assign_in(char *full_var, t_env **env)
{
	char	*key;
	char	*value;

	key = set_key(full_var);
	if (!key)
		return (1);
	value = set_value(full_var);
	if (!value)
	{
		free(key);
		return (1);
	}
	ft_assignation(key, value, env, full_var);
	free(key);
	free(value);
	return (0);
}

static void	ft_no_assignation(char *key, t_env **env)
{
	t_env	*new;
	t_env	*node;

	node = find_node_by_key(*env, key);
	if (!node)
	{
		new = ft_lstnew_mini_env(key, NULL, key);
		if (new)
			ft_addback_mini_env(env, new);
	}
	return ;
}

/**
 * @brief Checks if we're given the right format.
 * If there's an assignation, it creates or overwrites
 * an env variable.
 * If there's no assignation, an empty key is created.
 */
static int	navigate_args(char **args, t_env **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_not_valid(args[i]);
			i ++;
			continue ;
		}
		if (ft_strchr(args[i], '='))
		{
			if (ft_assign_in(args[i], env) == 1)
				return (1);
		}
		else
			ft_no_assignation(args[i], env);
		i++;
	}
	return (0);
}

/**
 * @brief Creates or redefines env variables.
 * If no parameters are given, it shows exported
 * env variables.
 */
int	ft_export(t_command *cmd, t_env **env)
{
	char	**args_arr;
	int		exit_status;

	if (!cmd || !env)
		return (1);
	args_arr = args_to_array(cmd->args);
	if (!args_arr)
		return (1);
	if (!args_arr[1])
	{
		print_sorted_env(*env);
		ft_free_array(args_arr);
		return (0);
	}
	exit_status = navigate_args(args_arr, env);
	ft_free_array(args_arr);
	return (exit_status);
}
