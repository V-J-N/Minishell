/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:08:49 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/28 13:29:30 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (false);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	print_sorted_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
	return ;
}

static void	ft_assignation(char *key, char *value, t_env **env, char **args)
{
	t_env	*node;
	t_env	*new;

	node = find_node_by_key(*env, key);
	if (node)
	{
		free(node->value);
		node->value = value;
		free(node->full_env);
		node->full_env = ft_strdup(args);
	}
	else
	{
		new = ft_lstnew_mini_env(key, value, args);
		free(value);
		if (new)
			ft_addback_mini_env(env, new);
	}
	return ;
}

static int	ft_assign_in(char *args, t_env **env, char **args_arr)
{
	char	*key;
	char	*value;
	

	key = set_key(args);
	if (!key)
	{
		ft_free_array(args);
		return (1);
	}
	value = set_value(args);
	if (!value)
	{
		free(key);
		ft_free_array(args_arr);
		return (1);
	}
	ft_assignation(key, value, env, args);
	free(key);
	return (0);
}

static void	ft_not_valid(char *args)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(args, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return ;
}

static void	ft_no_assignation(char *args, t_env **env)
{
	t_env	*new;
	t_env	*node;

	node = find_node_by_key(*env, args);
	if (!node)
	{
		new = ft_lstnew_mini_env(args, NULL, args);
		if (new)
			ft_addback_mini_env(env, new);
	}
	return ;
}

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
			if (ft_assign_in(args[i], env, args) == 1)
				return (1);
		}
		else
			ft_no_assignation(args[i], env);
		i++;
	}
	return (0);
}

int	ft_export(t_command *cmd, t_env **env)
{
	char	**args;
	int		exit_status;

	if (!cmd || !env)
		return (1);
	args = args_to_array(cmd->args);
	if (!args)
		return (1);
	if (!args[1])
	{
		print_sorted_env(*env);
		ft_free_array(args);
		return (0);
	}
	exit_status = navigate_args(args, env);
	ft_free_array(args);
	return (exit_status);
}
