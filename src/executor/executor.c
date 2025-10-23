/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:12:05 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/23 09:50:44 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	exec_child_process(t_command *command, t_env **env, int in, int out)
{
	int	new_in;
	int	new_out;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
 * @brief Unless a built_in function is called, 
 * a child process is started to execute a given command,
 * handling existing redirections.
*/
int	command_in(t_data *data, int in, int out)
{
	pid_t	pid;
	int		status;

	if (!data->parsed->cmd_list || !data->env)
		return (perror("No data"), EXIT_FAILURE);
	if (is_built_in(data->parsed->cmd_list->args->value))
		return (built_in(data->parsed->cmd_list->args->value, data, -1));
	pid = fork();
	if (pid < 0)
		return (perror("Fork"), EXIT_FAILURE);
	if (pid == 0)
		exec_child_process(data->parsed->cmd_list, &data->env, in, out);
	signal(SIGINT, SIG_IGN);
	status = ft_wait_and_exit(pid);
	signal(SIGINT, SIG_DFL);
	return (status);
}

/**
 * @brief Checks if we need to execute a single command block
 * or several ones through pipes, and returns the exit signal
 * of the process.
 * Prepares heredoc FDs before execution, and saves them in the
 * corresponding redir struct.
*/
int	execute_all(t_data *data)
{
	size_t	command_count;
	t_pipe	*pipe_data;
	int		exit_signal;

	if (!data->parsed->cmd_list || !data->env)
		return (perror("Missing data structures"), EXIT_FAILURE);
	if (!prepare_all_heredocs(data->parsed->cmd_list))
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
