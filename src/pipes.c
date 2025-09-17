/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:26:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/17 13:06:24 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_child_process(t_pipe *pipe_data, int prev_pipe, int *pipe_fd)
{
	int		new_in;
	int		new_out;
	char	**args;

	signal(SIGINT, SIG_DFL); // Restaurar señal en hijo

	// === DETERMINAR ENTRADA ===
	if (has_input_redir(pipe_data->commands))
		new_in = redirect_in(pipe_data->commands, STDIN_FILENO);
	else if (pipe_data->index == 0)
		new_in = pipe_data->in;
	else
		new_in = prev_pipe;

	// === DETERMINAR SALIDA ===
	if (has_output_redir(pipe_data->commands))
		new_out = redirect_out(pipe_data->commands, STDOUT_FILENO);
	else if (pipe_data->index == pipe_data->command_count - 1)
		new_out = pipe_data->out;
	else
		new_out = pipe_fd[1];

	// Si hubo error en redirecciones
	if (new_in == -1 || new_out == -1)
		exit(EXIT_FAILURE);

	// === DUPLICACIÓN DE FDs ===
	if (new_in != STDIN_FILENO)
	{
		if (dup2(new_in, STDIN_FILENO) == -1)
			perror("dup2 in"), exit(EXIT_FAILURE);
		safe_close(new_in);
	}
	if (new_out != STDOUT_FILENO)
	{
		if (dup2(new_out, STDOUT_FILENO) == -1)
			perror("dup2 out"), exit(EXIT_FAILURE);
		safe_close(new_out);
	}

	// === CERRAR FDs INNECESARIOS ===
	ft_close_three(pipe_data->in, pipe_data->out, prev_pipe);
	if (pipe_data->index < pipe_data->command_count - 1)
		ft_close_two(pipe_fd[0], pipe_fd[1]);

	// === EJECUTAR O REDIRECCIÓN SOLA ===
	args = command_to_arr(pipe_data->commands);
	if (!args || !args[0])
	{
		// No hay comando. Solo redirecciones.
		free(args);
		exit(EXIT_SUCCESS);
	}

	execute_command(pipe_data->commands, pipe_data->env_list);
	// Solo llegamos aquí si execve falla
	exit(EXIT_FAILURE);
}

/**
 * @brief Checks if we need to keep redirecting fds through pipes.
*/
static void	next_pipe(t_pipe *pipe_data, int *pipe_fd, int *prev_pipe)
{
	if (pipe_data->index < pipe_data->command_count - 1)
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

/**
 * @brief Pipe manager.
 */
int	pipes(t_pipe *pipe_data)
{
	int		pipe_fd[2];
	int		prev_pipe;
	pid_t	pid;
	pid_t	last_pid;
	int		status;

	prev_pipe = -1;
	last_pid = -1;
	while (pipe_data->index < pipe_data->command_count)
	{
		if (pipe_data->index < pipe_data->command_count - 1 && pipe(pipe_fd) == -1) // si no estamos en el último comando(blocks -1), necesitamos pipe
			return (perror("pipe"), EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
			signal(SIGINT, SIG_DFL), pipe_child_process(pipe_data, prev_pipe, pipe_fd);
		last_pid = pid;
		safe_close(prev_pipe); //cerrar descriptor que ya ha heredado el hijo
		next_pipe(pipe_data, pipe_fd, &prev_pipe);
		pipe_data->index ++;
		pipe_data->commands = pipe_data->commands->next;
	}
	ft_close_two(pipe_data->in, pipe_data->out);
	signal(SIGINT, SIG_IGN); // padre ignora Ctrl+C hasta que vuelve el hijo
	status = ft_wait_and_exit(last_pid);
	signal(SIGINT, SIG_DFL);//padre vuelve a hacer caso a la señal
	return (status);
}
