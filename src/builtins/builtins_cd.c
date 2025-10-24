/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:34:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/24 07:52:09 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_path(char **args, t_env *env)
{
	t_env	*home;

	if (!args[1])
	{
		home = find_node_by_key(env, "HOME");
		if (!home || !home->value)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (home->value);
	}
	if (args[1][0] == '\0')
		return (NULL);
	return (args[1]);
}

static int	update_oldpwd(t_env **env, char *cwd)
{
	char	*full;

	full = ft_strjoin("OLDPWD=", cwd);
	if (!full)
		return (1);
	if (ft_assign_in(full, env))
	{
		free(full);
		return (1);
	}
	free(full);
	return (0);
}

static int	update_pwd(t_env **env)
{
	char	cwd[4096];
	char	*full;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("cd: getcwd", STDERR_FILENO);
		return (1);
	}
	full = ft_strjoin("PWD=", cwd);
	if (!full)
		return (1);
	if (ft_assign_in(full, env))
	{
		free(full);
		return (1);
	}
	free(full);
	return (0);
}

/**
 * @brief Changes current directory to the one given.
 * If doing so, it updates the new PWD and the OLDPWD.
 */
int	ft_cd(t_command *cmd, t_env **env)
{
	char	cwd[4096];
	char	**args;
	char	*path;

	args = args_to_array(cmd->args);
	if (!args)
		return (1);
	path = get_target_path(args, *env);
	if (!path)
	{
		ft_free_array(args);
		return (0);
	}
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("cd: getcwd"), ft_free_array(args), 1);
	if (chdir(path) == -1)
		return (perror("cd"), ft_free_array(args), 1);
	if (update_oldpwd(env, cwd))
		return (ft_free_array(args), 1);
	if (update_pwd(env))
		return (ft_free_array(args), 1);
	ft_free_array(args);
	return (0);
}
