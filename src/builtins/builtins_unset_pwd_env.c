/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset_pwd_env.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:05:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 10:03:51 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_unset_pwd_env.c
 * @brief Implementation of the 'unset', 'pwd', and 'env' built-in commands.
 * 'unset' modifies the shell's environment. 'pwd' and 'env' are primarily
 * inspection commands.
 */
#include "minishell.h"

/**
 * @brief Utility function to safely update the PWD environment variable.
 * This function is used by `ft_pwd` as a backup mechanism if the PWD variable
 * is corrupt or missing, ensuring the shell's environment state remains
 * consistent. It leverages the `ft_assign_in` utility from `builtins_export.c`.
 * @param env A pointer to the head of the environment list.
 * @param cwd The canonical current working directory obtained via `getcwd()`.
 * @return int 0 for success, 1 for failure (memory or assignment error).
 */
static int	safety_update_pwd(t_env **env, char *cwd)
{
	char	*full;

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
 * @brief Implements the 'pwd' built-in command.
 * It prioritizes printing the value from the PWD environment variable
 * (if valid). If PWD is invalid (corrupt or deleted), it uses
 * `getcwd()` as a fallback and optionally attempts to update the PWD
 * variable in the environment.
 * @param env A double pointer to the environment list (t_env**).
 * @return int EXIT_SUCCESS or EXIT_FAILURE on system error.
 */
int	ft_pwd(t_env **env)
{
	char	*value;
	char	*cwd;

	if (!env)
		return (ft_putstr_fd("pwd: env error\n", STDERR_FILENO), EXIT_FAILURE);
	value = get_value_by_key(*env, "PWD");
	if (value && *value && access(value, F_OK) == 0)
	{
		ft_printf("%s\n", value);
		free(value);
		return (EXIT_SUCCESS);
	}
	free(value);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("pwd"), EXIT_FAILURE);
	if (safety_update_pwd(env, cwd))
		ft_putstr_fd("Failed to update PWD\n", 2);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}

/**
 * @brief Implements the 'env' built-in command.
 * Prints the environment variables to standard output in the standard
 * format (KEY=VALUE). Unlike `export`, it only prints variables that
 * have a value.
 * @param env The head of the environment list (t_env*).
 * @param cmd_lst The command node (used for argument counting).
 * @return int EXIT_SUCCESS or EXIT_FAILURE on error.
 */
int	ft_env(t_env *env, t_command *cmd_lst)
{
	t_env	*temp;

	if (!env)
	{
		ft_putstr_fd("env error\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd_lst->cmd_argc > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	temp = env;
	while (temp)
	{
		if (temp->value)
			ft_printf("%s\n", temp->full_env);
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Implements the 'unset' built-in command.
 * Removes one or more environment variables from the shell's environment
 * list. It validates each argument as an identifier and ignores invalid
 * ones, returning success (0) unless a memory error occurs.
 * @param cmd_lst The command node containing the arguments.
 * @param env A double pointer to the environment list head (t_env**).
 * @return int EXIT_SUCCESS or EXIT_FAILURE on memory error.
 */
int	ft_unset(t_command *cmd_lst, t_env **env)
{
	char	**args;
	int		i;

	if (!cmd_lst || !cmd_lst->args || !env)
		return (EXIT_FAILURE);
	args = args_to_array(cmd_lst->args);
	if (!args)
		return (EXIT_FAILURE);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: '", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		}
		else
			delete_env_key(env, args[i]);
		i++;
	}
	ft_free_array(args);
	return (EXIT_SUCCESS);
}
