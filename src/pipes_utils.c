/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:23:06 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/09 17:37:14 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int fd)
{
	if (fd > 2)
		close(fd);
}

void	ft_close_three(int fd1, int fd2, int fd3)
{
	safe_close(fd1);
	safe_close(fd2);
	safe_close(fd3);
	return ;
}

/// @brief esta parte recupera la señal del último proceso hijo. Réplica de: echo $?
int	ft_wait_and_exit(pid_t last_pid)
{
	int	status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0) //esperamos a todos los hijos
	{
		if (pid == last_pid) // pero sólo recuperamos el id del último
		{// Ahora usamos macros para extraer el código de salida de un proceso hijo:
			if (WIFEXITED(status))//se ha salido normalmente?
				return (WEXITSTATUS(status));
			else if (WIFSIGNALED(status))//o se ha terminado con señal?
				return (128 + WTERMSIG(status));//qué señal?
		}
	}
	return (EXIT_FAILURE); // Si no conseguimos capturar el último hijo por alguna razón
}

void	free_pipe_data(t_pipe *pipe_data)
{
	pipe_data->commands = NULL;
	pipe_data->command_count = 0;
	pipe_data->env_list = NULL;
	pipe_data->in = STDIN_FILENO;
	pipe_data->out = STDOUT_FILENO;
	pipe_data->index = 0;
	free(pipe_data);
}

t_pipe	*init_pipe_data(t_command *command, t_env **env_list, size_t nbr_of_commands)
{
	t_pipe	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe));
	if (!pipe_data)
		return (NULL);
	pipe_data->commands = command;
	pipe_data->command_count = nbr_of_commands;
	pipe_data->env_list = env_list;
	pipe_data->in = STDIN_FILENO;
	pipe_data->out = STDOUT_FILENO;
	pipe_data->index = 0;
	return (pipe_data);
}
