/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:34:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/31 20:22:33 by serjimen         ###   ########.fr       */
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
	if (args[2])
	{
		ft_putstr_fd("cd: Too many arguments\n", STDERR_FILENO);
		return (NULL);
	}
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
		return (EXIT_FAILURE);
	}
	free(full);
	return (EXIT_SUCCESS);
}

static int	update_pwd(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*full;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("cd: getcwd\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	full = ft_strjoin("PWD=", cwd);
	if (!full)
		return (EXIT_FAILURE);
	if (ft_assign_in(full, env))
	{
		free(full);
		return (EXIT_FAILURE);
	}
	free(full);
	return (EXIT_SUCCESS);
}

/**
 * @brief Changes current directory to the one given.
 * If doing so, it updates the new PWD and the OLDPWD.
 */
int	ft_cd(t_command *cmd, t_env **env)
{
	char	cwd[PATH_MAX];
	char	**args;
	char	*path;

	args = args_to_array(cmd->args);
	if (!args)
		return (EXIT_FAILURE);
	path = get_target_path(args, *env);
	if (!path)
	{
		ft_free_array(args);
		return (EXIT_FAILURE);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("cd: getcwd\n", STDERR_FILENO);
		cwd[0] = '\0';
	}
	if (chdir(path) == -1)
		return (perror("chdir"), ft_free_array(args), EXIT_FAILURE);
	if (update_oldpwd(env, cwd))
		return (ft_free_array(args), EXIT_FAILURE);
	if (update_pwd(env))
		return (ft_free_array(args), EXIT_FAILURE);
	return (ft_free_array(args), EXIT_SUCCESS);
}
