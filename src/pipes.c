/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:26:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/04 13:48:53 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_pipe *pipe_data, int prev_pipe, int *pipe_fd)
{
	if (pipe_data->index == 0)//si es el primero, usar in
	{
		if (dup2(pipe_data->in, 0) == -1)
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	else//si no, usar anterior pipe
	{
		if (dup2(prev_pipe, 0) == -1)
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	if (pipe_data->index == pipe_data->blocks - 1)
	{
		if (dup2(pipe_data->out, 1) == -1)// si es el último comando, escribir en out
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	else
	{
		if (dup2(pipe_fd[1], 1) == -1)// si no, escribir en pipe actual para que el siguiente proceso lo lea
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	ft_close_three(pipe_data->in, pipe_data->out, prev_pipe);
	if (pipe_data->index < pipe_data->blocks - 1)//si no estamos en el último comando, cerrar pipe actual
		ft_close_two(pipe_fd[0], pipe_fd[1]);
	execute_command(pipe_data->args[pipe_data->index], pipe_data->env_list);
	exit(EXIT_FAILURE);
}

static void	next_pipe(t_pipe *pipe_data, int *pipe_fd, int *prev_pipe)
{
	if (pipe_data->index < pipe_data->blocks - 1)
	{
		close(pipe_fd[1]); //ya hemos escrito aquí
		*prev_pipe = pipe_fd[0]; //de aquí leerá el próximo comando
	}
	else
	{
		if (pipe_fd[0] != -1 && pipe_fd[1] != -1)
			ft_close_two(pipe_fd[0], pipe_fd[1]);// Último comando, ya no necesitamos pipe_fds
	}
	return ;
}

/// @brief Pipe manager:
int	pipes(t_pipe *pipe_data, int prev_pipe)
{
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	last_pid;

	last_pid = -1;
	while (pipe_data->index < pipe_data->blocks)
	{
		if (pipe_data->index < pipe_data->blocks - 1 && pipe(pipe_fd) == -1) // si no estamos en el último comando(blocks -1), necesitamos pipe
			return (perror("pipe"), EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
			child_process(pipe_data, prev_pipe, pipe_fd);
		last_pid = pid;
		safe_close(prev_pipe); //cerrar descriptor que ya ha heredado el hijo
		next_pipe(pipe_data, pipe_fd, &prev_pipe);
		pipe_data->index ++;
	}
	ft_close_two(pipe_data->in, pipe_data->out);
	return (ft_wait_and_exit(last_pid));
}
