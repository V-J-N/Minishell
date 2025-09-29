/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/29 06:55:27 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char *cmd, t_env *env, t_command *cmd_lst)
{
	
	if (!(ft_strncmp(cmd, "echo", 5)))
		return (ft_echo(cmd_lst));
	else if (!(ft_strncmp(cmd, "pwd", 4)))
		return (ft_pwd(env));
	else if (!(ft_strncmp(cmd, "env", 4)))
		return (ft_env(env, cmd_lst));
	else if (!(ft_strncmp(cmd, "exit", 5)))
		return (ft_exit(cmd_lst));
	else if (!(ft_strncmp(cmd, "unset", 6)))
		return (ft_unset(cmd_lst, &env));
	else if (!(ft_strncmp(cmd, "export", 7)))
		return (ft_export(cmd_lst, &env));
	else if (!(ft_strncmp(cmd, "cd", 3)))
		return (ft_cd(cmd_lst, &env));
	return (-1);
}

int	built_in(char *cmd, t_env *env, t_command *cmd_lst)
{
	pid_t	pid;
	int		exit_return;

	if (is_parent_built_in(cmd))
		return (execute_builtin(cmd, env, cmd_lst));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			exit_return = execute_builtin(cmd, env, cmd_lst);
			if (exit_return == -1)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			exit_return = ft_wait_and_exit(pid);
			signal(SIGINT, SIG_DFL);
			return (exit_return);
		}
			
	}
	return (-1);
}

/** @brief Prints PWD from env_list.
 * @warning No sé cómo debería actuar si PWD
 * está corrupto. Uso getcwd como backup pero
 * no sé si debería dejarlo romperse en paz.
 */
int	ft_pwd(t_env *env)
{
	char	*value;
	char	*cwd;

	if (!env)
		return (perror("env error"), EXIT_FAILURE);
	value = get_value_by_key(env, "PWD");
	if (value && *value && access(value, F_OK) == 0) //si archivo o directorio existe
	{
		ft_printf("%s\n", value);
		free(value);
		return (EXIT_SUCCESS);
	}
	free(value);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("getcwd error"), EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}

/** @brief Shows in terminal the environment variables
 * stored in the t_env list.
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
		ft_printf("%s\n", temp->full_env);
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Removes environment variables based on given keys.
 */
int	ft_unset(t_command *cmd_lst, t_env **env)
{
	char	**args;
	int		i;

	if (!cmd_lst || !cmd_lst->args || !env)
		return (1);
	args = args_to_array(cmd_lst->args);
	if (!args)
		return (1);
	i = 1;
	while (args[i])
	{
		delete_env_key(env, args[i]);
		i++;
	}
	ft_free_array(args);
	return (0);
}
