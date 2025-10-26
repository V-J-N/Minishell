/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 09:03:24 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Checks which built_in to call
 * and returns an int as an exit signal.
 *  */
int	execute_builtin(char *cmd, t_data *data)
{
	if (!(ft_strncmp(cmd, "echo", 5)))
		return (ft_echo(data->parsed->cmd_list));
	else if (!(ft_strncmp(cmd, "pwd", 4)))
		return (ft_pwd(data->env));
	else if (!(ft_strncmp(cmd, "env", 4)))
		return (ft_env(data->env, data->parsed->cmd_list));
	else if (!(ft_strncmp(cmd, "exit", 5)))
		return (ft_exit(data));
	else if (!(ft_strncmp(cmd, "unset", 6)))
		return (ft_unset(data->parsed->cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "export", 7)))
		return (ft_export(data->parsed->cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "cd", 3)))
		return (ft_cd(data->parsed->cmd_list, &data->env));
	return (-1);
}

static void	builtin_c_process(char *cmd, t_data *data, int new_in, int new_out)
{
	int	exit_return;

	signal(SIGINT, SIG_DFL);
	exit_return = -1;
	new_in = redirect_in(data->parsed->cmd_list, STDIN_FILENO);
	new_out = redirect_out(data->parsed->cmd_list, STDOUT_FILENO);
	if (new_in == -1 || new_out == -1)
		exit(EXIT_FAILURE);
	if (new_in != STDIN_FILENO)
	{
		dup2(new_in, STDIN_FILENO);
		close(new_in);
	}
	if (new_out != STDOUT_FILENO)
	{
		dup2(new_out, STDOUT_FILENO);
		close(new_out);
	}
	exit_return = execute_builtin(cmd, data);
	if (exit_return == -1)
		exit(EXIT_FAILURE);
	else
		exit(exit_return);
}

/**
 * @brief Checks if the execution should be done
 * in a child process or in the main one, and 
 * returns an int as an exit signal.
 */
int	built_in(char *cmd, t_data *data, int exit_return)
{
	pid_t	pid;

	if (is_parent_built_in(cmd))
		return (execute_builtin(cmd, data));
	else
	{
		pid = fork();
		if (pid == 0)
			builtin_c_process(cmd, data, -1, -1);
		else
		{
			signal(SIGINT, SIG_IGN);
			exit_return = ft_wait_and_exit(pid);
			signal(SIGINT, sigint_handler);
			return (exit_return);
		}
	}
	return (exit_return);
}
