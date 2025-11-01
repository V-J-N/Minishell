/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:34:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/31 20:40:19 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_cd.c
 * @brief Implementation of the 'cd' built-in command, including environment
 * variable updates (PWD and OLDPWD).
 */
#include "minishell.h"

/**
 * @brief Determines the target directory path based on the 'cd' arguments.
 * This function handles the standard `cd` logic:
 * 1 - No argument: Defaults to the HOME directory.
 * 2 - Too many arguments: Prints an error and returns NULL.
 * 3 - Empty string argument: Returns NULL (no change).
 * @param args The null-terminated array of arguments.
 * @param env The head of the environment list.
 * @return char* The target path string. Returns NULL on error or if no
 * action is needed.
 */
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

/**
 * @brief Updates the OLDPWD environment variable with the previous
 * working directory.
 * This involves creating the string "OLDPWD=<cwd>", and using the environment
 * assignment utility (`ft_assign_in`) to update or create the variable.
 * @param env A pointer to the head of the environment list.
 * @param cwd The string of the current working directory BEFORE the
 * 'cd' operation.
 * @return int EXIT_SUCCESS or EXIT_FAILURE on memory allocation/assignment
 * error.
 */
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

/**
 * @brief Updates the PWD environment variable with the new current
 * working directory.
 * It retrieves the current directory using `getcwd()` and updates the PWD
 * variable in the environment list.
 * @param env A pointer to the head of the environment list.
 * @return int EXIT_SUCCESS or EXIT_FAILURE on system call or allocation error.
 */
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
 * @brief Implements the 'cd' built-in command.
 * Changes the shell's current working directory (CWD) and updates the
 * OLDPWD and PWD environment variables accordingly. Because it modifies
 * the shell's state, it must run in the parent process.
 * @param cmd A pointer to the 't_command' node containing the arguments.
 * @param env A double pointer to the environment list head (t_env**).
 * @return int The exit status of the command (0 on success, 1 on failure).
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
		return (perror("cd"), ft_free_array(args), EXIT_FAILURE);
	if (update_oldpwd(env, cwd))
		return (ft_free_array(args), EXIT_FAILURE);
	if (update_pwd(env))
		return (ft_free_array(args), EXIT_FAILURE);
	return (ft_free_array(args), EXIT_SUCCESS);
}
