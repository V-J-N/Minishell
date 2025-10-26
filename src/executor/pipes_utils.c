/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:23:06 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 10:47:16 by vjan-nie         ###   ########.fr       */
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

void	ft_close_two(int fd1, int fd2)
{
	safe_close(fd1);
	safe_close(fd2);
	return ;
}

void	free_pipe_data(t_pipe *pipe_data)
{
	if (!pipe_data)
		return ;
	if (pipe_data->in > 2)
		close(pipe_data->in);
	if (pipe_data->out > 2)
		close(pipe_data->out);
	pipe_data->data = NULL;
	free(pipe_data);
}

/**
 * @brief Initialization of pipe_data structure
 * containing what we need in order to execute trhough
 * pipes.
*/
t_pipe	*init_pipe_data(t_data *data, size_t commands)
{
	t_pipe	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe));
	if (!pipe_data)
		return (NULL);
	pipe_data->commands = data->parsed->cmd_list;
	pipe_data->current_command = pipe_data->commands;
	pipe_data->command_count = commands;
	pipe_data->env_list = &data->env;
	pipe_data->in = STDIN_FILENO;
	pipe_data->out = STDOUT_FILENO;
	pipe_data->index = 0;
	pipe_data->data = data;
	return (pipe_data);
}
