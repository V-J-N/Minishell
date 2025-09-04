/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:23:06 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/02 17:08:42 by vjan-nie         ###   ########.fr       */
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
	pipe_data->args = NULL;
	pipe_data->blocks = 0;
	pipe_data->env_list = NULL;
	pipe_data->in = 0;
	pipe_data->out = 0;
	pipe_data->index = 0;
	free(pipe_data);
}

t_pipe	*init_pipe_data(char **pipe_args, t_env **env_list, int in, int out)
{
	int		num_blocks;
	t_pipe	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe));
	if (!pipe_data)
		return (NULL);
	num_blocks = 0;
	while (pipe_args[num_blocks])// número de bloques de comandos
		num_blocks++;
	if (in == -1)//detectamos si estamos usando FDs especiales (si hemos incorporado redirecciones, etc.)
		in = 0;//si no, usamos std in and out
	if (out == -1)
		out = 1;
	pipe_data->args = pipe_args;
	pipe_data->blocks = num_blocks;
	pipe_data->env_list = env_list;
	pipe_data->in = in;
	pipe_data->out = out;
	pipe_data->index = 0;
	return (pipe_data);
}
