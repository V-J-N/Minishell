/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/02 01:17:48 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins.c
 * @brief Central dispatch and execution logic for all built-in commands.
 * This file contains routines to identify a built-in command, decide whether
 * it should run in the main shell process or a subprocess.
 */
#include "minishell.h"

/**
 * @brief Dispatches the execution to the corresponding built-in handler
 * function.
 * This function serves as the central router for all known internal commands,
 * using strict string comparison to match the command name.
 * @param cmd The command string.
 * @param cmd_list A pointer to the current 't_command' node containing
 * arguments and redirections.
 * @param data A pointer to the main 't_data' structure.
 * @return int The exit status of the built-in command. 0 for success, >0 for
 * failure, -1 if the command string does not match any recognized built-in.
 */
int	execute_builtin(char *cmd, t_command *cmd_list, t_data *data, bool pipe)
{
	if (!(ft_strncmp(cmd, "echo", 5)))
		return (ft_echo(cmd_list));
	else if (!(ft_strncmp(cmd, "pwd", 4)))
		return (ft_pwd(&data->env));
	else if (!(ft_strncmp(cmd, "env", 4)))
		return (ft_env(data->env, cmd_list));
	else if (!(ft_strncmp(cmd, "exit", 5)))
		return (ft_exit(data, cmd_list, pipe));
	else if (!(ft_strncmp(cmd, "unset", 6)))
		return (ft_unset(cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "export", 7)))
		return (ft_export(cmd_list, &data->env));
	else if (!(ft_strncmp(cmd, "cd", 3)))
		return (ft_cd(cmd_list, &data->env));
	return (-1);
}

/**
 * @brief Execution routine for built-ins running in a child process.
 * This function is executed immediately after fork() in the child process.
 * It is responsible for:
 * 1 - Setting default signal handling.
 * 2 - Aplying command-specific redirections.
 * 3 - Executing the built-in via 'execute_builtin'.
 * 4 - Terminating the child process with the built-in's exit status.
 * @param cmd_lst A pointer to the command node to execute.
 * @param data A pointer to the main data structure.
 * @param n_in Placeholder for input file descriptor.
 * @param n_out Placeholder for output file descriptor.
 * @note This function guarantees process termination via `exit()`.
 */
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
	exit_return = execute_builtin(cmd, cmd_lst, data, 1);
	if (exit_return == -1)
		exit(EXIT_FAILURE);
	else
		exit(exit_return);
}

/**
 * @brief Determines the execution context for a built-in command.
 * Built-ins that modify the shell's state (`cd`, `export`, `unset`, `exit`)
 * must run in the parent process to ensure changes persist.
 * Other built-ins (`echo`, `pwd`, `env`) typically run in a child
 * process unless they are part of a pipeline.
 * @param cmd_list A pointer to the command node to execute.
 * @param data A pointer to the main data structure.
 * @param exit_return Initial exit status.
 * @param pipe Boolean flag: true if the command is part of a pipeline.
 * @return int The final exit status of the built-in command.
 */
int	built_in(t_command *cmd_list, t_data *data, int exit_return, bool pipe)
{
	pid_t	pid;
	char	*cmd;

	cmd = cmd_list->args->value;
	if (is_parent_built_in(cmd) || pipe)
		return (execute_builtin(cmd, cmd_list, data, pipe));
	else
	{
		pid = fork();
		if (pid == 0)
			blt_c_procss(cmd_list, data, -1, -1);
		else
		{
			signal(SIGINT, SIG_IGN);
			exit_return = ft_wait_and_exit(pid);
			if (g_exit_code == SIGINT)
				ft_putstr_fd("\n", 1);
			setup_signals();
			return (exit_return);
		}
	}
	return (exit_return);
}
