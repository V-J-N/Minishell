/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 12:06:31 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Checks which built_in to call
 * and returns an int as an exit signal.
 *  */
int	execute_builtin(char *cmd, t_command *cmd_list, t_data *data)
{
	if (!(ft_strncmp(cmd, "echo", 5)))
		return (ft_echo(cmd_list));
	else if (!(ft_strncmp(cmd, "pwd", 4)))
		return (ft_pwd(data->env));
	else if (!(ft_strncmp(cmd, "env", 4)))
		return (ft_env(data->env, cmd_list));
	else if (!(ft_strncmp(cmd, "exit", 5)))
		return (ft_exit(data, cmd_list));
	else if (!(ft_strncmp(cmd, "unset", 6)))
		return (ft_unset(cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "export", 7)))
		return (ft_export(cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "cd", 3)))
		return (ft_cd(cmd_list, &data->env));
	return (-1);
}

static void	blt_c_procss(t_command *cmd_lst, t_data *data, int n_in, int n_out)
{
	int		exit_return;
	char	*cmd;

	cmd = cmd_lst->args->value;
	signal(SIGINT, SIG_DFL);
	exit_return = -1;
	n_in = redirect_in(data->parsed->cmd_list, STDIN_FILENO);
	n_out = redirect_out(data->parsed->cmd_list, STDOUT_FILENO);
	if (n_in == -1 || n_out == -1)
		exit(EXIT_FAILURE);
	if (n_in != STDIN_FILENO)
	{
		dup2(n_in, STDIN_FILENO);
		close(n_in);
	}
	if (n_out != STDOUT_FILENO)
	{
		dup2(n_out, STDOUT_FILENO);
		close(n_out);
	}
	exit_return = execute_builtin(cmd, cmd_lst, data);
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
int	built_in(t_command *cmd_list, t_data *data, int exit_return, bool pipe)
{
	pid_t	pid;
	char	*cmd;

	cmd = cmd_list->args->value;
	if (is_parent_built_in(cmd) || pipe)
		return (execute_builtin(cmd, cmd_list, data));
	else
	{
		pid = fork();
		if (pid == 0)
			blt_c_procss(cmd_list, data, -1, -1);
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
