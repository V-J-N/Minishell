/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:26:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/03 14:02:50 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (pipe_data->index == pipe_data->command_count - 1)
			last_pid = pid;
		safe_close(prev_pipe);
		next_pipe(pipe_data, pipe_fd, &prev_pipe);
		if (pipe_data->commands)
			pipe_data->commands = pipe_data->commands->next;
		pipe_data->index++;
	}
	return (pipes_return(pipe_data, last_pid));
}
