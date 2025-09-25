/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:26:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 11:36:52 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_in_out_fds(int new_in, int new_out)
{
	if (new_in == -1 || new_out == -1)
		exit(EXIT_FAILURE);
	if (new_in != STDIN_FILENO)
	{
		if (dup2(new_in, STDIN_FILENO) == -1)
		{
			perror("dup2 in");
			exit(EXIT_FAILURE);
		}
		safe_close(new_in);
	}
	if (new_out != STDOUT_FILENO)
	{
		if (dup2(new_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			exit(EXIT_FAILURE);
		}
		safe_close(new_out);
	}
}

/**
 * @brief Handles redirections and pipe ends before executing command.
*/
static void	p_child_process(t_pipe *pipe_data, int prev_pipe, int *pipe_fd)
{
	int		new_in;
	int		new_out;

	signal(SIGINT, SIG_DFL);
	if (has_input_redir(pipe_data->commands))
		new_in = redirect_in(pipe_data->commands, STDIN_FILENO);
	else if (pipe_data->index == 0)
		new_in = pipe_data->in;
	else
		new_in = prev_pipe;
	if (has_output_redir(pipe_data->commands))
		new_out = redirect_out(pipe_data->commands, STDOUT_FILENO);
	else if (pipe_data->index == pipe_data->command_count - 1)
		new_out = pipe_data->out;
	else
		new_out = pipe_fd[1];
	handle_in_out_fds(new_in, new_out);
	ft_close_three(pipe_data->in, pipe_data->out, prev_pipe);
	if (pipe_data->index < pipe_data->command_count - 1)
		ft_close_two(pipe_fd[0], pipe_fd[1]);
	if (is_built_in(pipe_data->commands->args->value))
		exit(built_in(pipe_data->commands->args->value, *pipe_data->env_list, pipe_data->commands));
	execute_command(pipe_data->commands, pipe_data->env_list);
	exit(EXIT_FAILURE);
}

/**
* @brief Checks if we need to keep redirecting fds through pipes
or if we're in the last block of commands, in which case we don't
need the pipe.
*/
static void	next_pipe(t_pipe *pipe_data, int *pipe_fd, int *prev_pipe)
{
	if (pipe_data->index < pipe_data->command_count - 1)
	{
		close(pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
	else
	{
		if (pipe_fd[0] != -1 && pipe_fd[1] != -1)
			ft_close_two(pipe_fd[0], pipe_fd[1]);
	}
	return ;
}

static int	pipes_return(t_pipe *pipe_data, pid_t last_pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	ft_close_two(pipe_data->in, pipe_data->out);
	status = ft_wait_and_exit(last_pid);
	signal(SIGINT, SIG_DFL);
	return (status);
}

/**
 * @brief Pipe manager. Starts child processes in each block
 * of commands, piping them and closing non-used read-write ends.
 */
int	pipes(t_pipe *pipe_data)
{
	int		pipe_fd[2];
	int		prev_pipe;
	pid_t	pid;
	pid_t	last_pid;

	prev_pipe = -1;
	last_pid = -1;
	while (pipe_data->index < pipe_data->command_count)
	{
		if (pipe_data->index < pipe_data->command_count - 1 \
		&& pipe(pipe_fd) == -1)
			return (perror("pipe"), EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
			p_child_process(pipe_data, prev_pipe, pipe_fd);
		last_pid = pid;
		safe_close(prev_pipe);
		next_pipe(pipe_data, pipe_fd, &prev_pipe);
		pipe_data->index ++;
		pipe_data->commands = pipe_data->commands->next;
	}
	return (pipes_return(pipe_data, last_pid));
}
