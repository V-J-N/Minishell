/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:23:06 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/22 14:56:50 by vjan-nie         ###   ########.fr       */
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
	free(pipe_data);
}

/**
 * @brief Initialization of pipe_data structure
 * containing what we need in order to execute trhough
 * pipes.
*/
t_pipe	*init_pipe_data(t_command *command, t_env **env_list, size_t commands)
{
	t_pipe	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe));
	if (!pipe_data)
		return (NULL);
	pipe_data->commands = command;
	pipe_data->command_count = commands;
	pipe_data->env_list = env_list;
	pipe_data->in = STDIN_FILENO;
	pipe_data->out = STDOUT_FILENO;
	pipe_data->index = 0;
	return (pipe_data);
}
