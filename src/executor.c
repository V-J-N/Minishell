/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:12:05 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/10 12:07:34 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execution of command if there's a 
 * valid path inside a child process. Exit with
 * corresponding error.
*/
void	execute_command(t_command *command, t_env **envlist)
{
	char	**args;
	char	*full_path;
	char	**env_arr;

	env_arr = envlist_to_arr(envlist);
	if (!env_arr)
		exit(1);//error genérico
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
	ft_free_array(args);
	ft_free_array(env_arr);
	free(full_path);
	perror("execve");
	if (errno == ENOENT)
		exit(127); // No existe el binario
	else if (errno == EACCES)
		exit(126); // Permiso denegado
	else
		exit(1); // Otro error genérico
}

/** 
 * @brief A child process is started to execute a given command,
 * handling existing redirections.
*/
int	command_in(t_command *command, t_env **environment, int in, int out)
{
	pid_t	pid;
	int 	new_in;
	int 	new_out;
	int		status;

	if (!command || !environment)
		return (perror("No data"), EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		return (perror("Fork"), EXIT_FAILURE);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // hijo reacciona a Ctrl+C
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
		execute_command(command, environment);
	}
	signal(SIGINT, SIG_IGN); // padre ignora Ctrl+C hasta que hijo termine
	status = ft_wait_and_exit(pid);
	signal(SIGINT, SIG_DFL);//padre vuelve a hacer caso a la señal
	return (status);
}

/**
 * @brief Checks if we need to execute a single command block
 * or several ones through pipes, and returns the exit signal
 * of the process.
*/
int	execute_all(t_command *commands, t_env **environment)
{
	size_t	command_count;
	t_pipe	*pipe_data;
	int		exit_signal;

	if (!commands || !environment)
		return (perror("Missing data structures"), EXIT_FAILURE);
	command_count = number_of_commands(commands);
	if (command_count < 1)
		return (perror("No command"), EXIT_FAILURE);
	if (command_count == 1)
		return (command_in(commands, environment, STDIN_FILENO, STDOUT_FILENO));
	pipe_data = init_pipe_data(commands, environment, command_count);
	if (!pipe_data)
		return (perror("pipe_data error"), EXIT_FAILURE);
	exit_signal = pipes(pipe_data);
	free_pipe_data(pipe_data);
	return (exit_signal);
}
