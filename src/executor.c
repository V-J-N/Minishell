/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:12:05 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/09 17:39:09 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_command *command, t_env **envlist)
{
	char	**args;
	char	*full_path;
	char	**env_arr;

	env_arr = envlist_to_arr(envlist);
	if (!env_arr)
		return ;
	args = command_to_arr(command);
	if (!args || !args[0])
	{
		ft_free_array(env_arr);
		return ;
	}
	full_path = ft_check_path(args[0], env_arr);
	if (!full_path)
		cmd_not_found(args[0], args);
	execve(full_path, args, env_arr);
	perror("execve");
	ft_free_array(args);
	ft_free_array(env_arr);
	free(full_path);
	exit (1);
}

int	command_in(t_command *command, t_env **environment, int in, int out)
{
	pid_t	pid;
	pid_t	last_pid;
	t_redir	*redir;
	int		new_in;
	int		new_out;

	if (!command || !environment)
		return (perror("No data"), EXIT_FAILURE);
	pid = -1;
	last_pid = -1;
	redir = command->redirs;
	if (!redir)
	{
		pid = fork();
		if (pid < 0)
			return (perror("Fork"), EXIT_FAILURE);
		if (pid == 0)
			execute_command(command, environment);
		return (ft_wait_and_exit(pid));
	}
	while(redir)
	{
		pid = fork();
		if (pid < 0)
			return (perror("Fork"), EXIT_FAILURE);
		if (pid == 0)
		{
			new_in = redirect_in(command, in);
			new_out = redirect_out(command, out);
			if (in != new_in)
			{
				if (dup2(new_in, in) == -1)
					return(perror("dup2 in"), EXIT_FAILURE);
				close(in);
			}
			if (out != new_out)
			{
				if (dup2(new_out, out) == -1)
					return(perror("dup2 out"), EXIT_FAILURE);
				close(out);
			}
			execute_command(command, environment);
		}
		last_pid = pid;
		redir = redir->next;
	}
	return (ft_wait_and_exit(last_pid));
}

/// @brief Initiate a child process to execute a given command
int	execute_all(t_command *commands, t_env **environment)
{
	size_t	command_count;
	t_pipe	*pipe_data;
	int		exit_signal;

	if (!commands || !environment)//Qué error sería este?
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
