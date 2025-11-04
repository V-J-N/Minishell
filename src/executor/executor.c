/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:12:05 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/04 01:09:56 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file executor.c
 * @brief Main logic for command execution, process management (fork/wait),
 * I/O redirection setup, and handling of built-in functions.
 */
#include "minishell.h"

/**
 * @brief Handles the cleanup and exit sequence for a child process upon
 * execve failure.
 * It frees all memory associated with the execution (args, environment array,
 * path) and exits with the appropriate status code based on errno.
 * @param args Array of arguments passed to execve.
 * @param env_arr Array of environment variables passed to execve.
 * @param full_path The resolved path to the executable.
 */
static void	ft_execve_failed(char **args, char **env_arr, char *full_path)
{
	ft_free_array(args);
	ft_free_array(env_arr);
	free(full_path);
	perror("execve");
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES)
		exit(126);
	else
		exit(1);
}

/**
 * @brief Execution of command if there's a valid path inside a child process.
 * If execve fails, ft_execve_failed handles a clean exit of the child process.
 * Steps:
 * 1. Convert lists to arrays. 2. Find executable path. 3. Execute via execve.
 * @param command The command node to execute.
 * @param envlist Pointer to the head of the environment list (t_env **).
 */
void	execute_command(t_command *command, t_env **envlist)
{
	char	**args;
	char	*full_path;
	char	**env_arr;

	env_arr = envlist_to_arr(envlist);
	if (!env_arr)
		exit(1);
	args = command_to_arr(command);
	if (!args || !args[0])
	{
		ft_free_array(env_arr);
		exit(1);
	}
	full_path = ft_check_path(args[0], env_arr);
	if (!full_path)
		cmd_not_found(args[0], env_arr, args);
	execve(full_path, args, env_arr);
	ft_execve_failed(args, env_arr, full_path);
}

/**
 * @brief Sets up I/O redirections for a child process and executes the command.
 * This function handles `dup2` for standard I/O based on the command's
 * redirections and the pipe file descriptors (`in` and `out`).
 * @param command The command node to execute.
 * @param env Pointer to the environment list.
 * @param in File descriptor for input (STDIN_FILENO or read end of a pipe).
 * @param out File descriptor for output (STDOUT_FILENO or write end of a pipe).
 */
static void	exec_child_process(t_command *command, t_env **env, int in, int out)
{
	int	new_in;
	int	new_out;

	signal(SIGINT, SIG_DFL);
	new_in = redirect_in(command, in);
	new_out = redirect_out(command, out);
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
	execute_command(command, env);
	return ;
}

/** 
 * @brief Executes a single command block (without pipes) by forking a child
 * process or by directly executing a built-in function.
 * @param data Pointer to the main data structure.
 * @param in Input file descriptor (usually STDIN_FILENO).
 * @param out Output file descriptor (usually STDOUT_FILENO).
 * @return int The exit status of the executed command.
 */
int	command_in(t_data *data, int in, int out)
{
	pid_t	pid;
	int		status;

	if (!data->parsed->cmd_list || !data->env)
		return (perror("No data"), EXIT_FAILURE);
	if (is_built_in(data->parsed->cmd_list->args->value))
		return (built_in(data->parsed->cmd_list, data, -1, 0));
	pid = fork();
	if (pid < 0)
		return (perror("Fork"), EXIT_FAILURE);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
		exec_child_process(data->parsed->cmd_list, &data->env, in, out);
	status = ft_wait_and_exit(pid);
	if (g_exit_code == SIGINT)
		ft_putstr_fd("\n", 1);
	setup_signals();
	return (status);
}

/**
 * @brief Main execution entry point. It prepares heredocs, determines if
 * execution is a single command or a pipeline, and delegates accordingly.
 * @param data Pointer to the main data structure.
 * @param exit_signal The previous exit status (can be used for $?).
 * @return int The final exit status of the executed command(s).
 */
int	execute_all(t_data *data, int exit_signal)
{
	size_t	command_count;
	t_pipe	*pipe_data;

	if (!data->parsed->cmd_list || !data->env)
		return (perror("Missing data structures"), EXIT_FAILURE);
	if (!prepare_all_heredocs(data->parsed->cmd_list, data, exit_signal))
		return (EXIT_FAILURE);
	command_count = number_of_commands(data->parsed->cmd_list);
	if (command_count < 1)
		return (perror("No command"), EXIT_FAILURE);
	if (command_count == 1)
	{
		if (!data->parsed->cmd_list->args && has_redirs(data->parsed->cmd_list))
			return (redirection_only(data->parsed->cmd_list, \
			STDIN_FILENO, STDOUT_FILENO));
		return (command_in(data, STDIN_FILENO, STDOUT_FILENO));
	}
	pipe_data = init_pipe_data(data, command_count);
	if (!pipe_data)
		return (perror("pipe_data error"), EXIT_FAILURE);
	exit_signal = pipes(pipe_data, -1, -1);
	free_pipe_data(pipe_data);
	return (exit_signal);
}
