/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:08:49 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 13:32:39 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(const char *s)
{
	int	i;

	i = 0;
	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (false);
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
}

int	ft_export(t_command *cmd, t_env **env)
{
	char	**args;
	t_env	*node;
	char	*key;
	char	*value;
	int		i;
	t_env	*new;

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
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			continue ;
		}
		key = set_key(args[i]);
		value = ft_strchr(args[i], '=') ? set_value(args[i]) : NULL;
		node = find_node_by_key(*env, key);
		if (node)
		{
			if (value)
			{
				free(node->value);
				node->value = value;
				free(node->full_env);
				node->full_env = ft_strdup(args[i]);
			}
			else
				free(value);
			free(key);
		}
		else
		{
			new = ft_lstnew_mini_env(key, value, args[i]);
			ft_addback_mini_env(env, new);
			free(key);
		}
		i++;
	}
	ft_free_array(args);
	return (0);
}
