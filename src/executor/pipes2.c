/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:33:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/23 07:20:56 by vjan-nie         ###   ########.fr       */
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

static int	get_new_in(t_pipe *pipe_data, int prev)
{
	if (has_input_redir(pipe_data->commands))
		return (redirect_in(pipe_data->commands, STDIN_FILENO));
	if (pipe_data->index == 0)
		return (pipe_data->in);
	return (prev);
}

static int	get_new_out(t_pipe *pipe_data, int *pipe_fd)
{
	if (has_output_redir(pipe_data->commands))
		return (redirect_out(pipe_data->commands, STDOUT_FILENO));
	if (pipe_data->index == pipe_data->command_count - 1)
		return (pipe_data->out);
	return (pipe_fd[1]);
}

static void	exec_child(t_pipe *pipe_data, int new_in, int new_out, int *pipe_fd)
{
	t_command	*cmd;

	handle_in_out_fds(new_in, new_out);
	ft_close_three(pipe_data->in, pipe_data->out, new_in);
	if (pipe_data->index < pipe_data->command_count - 1)
		ft_close_two(pipe_fd[0], pipe_fd[1]);
	cmd = pipe_data->commands;
	if ((!cmd->args || !cmd->args->value) && has_redirs(cmd))
		exit(EXIT_SUCCESS);
	if (cmd->args && cmd->args->value && is_built_in(cmd->args->value))
		exit(built_in(cmd->args->value, pipe_data->data, -1));
	if (cmd->args && cmd->args->value)
		execute_command(pipe_data->commands, pipe_data->env_list);
	exit(EXIT_FAILURE);
}

/**
 * @brief Handles redirections and pipe ends before executing command.
*/
void	p_child_process(t_pipe *pipe_data, int prev, int *pipe_fd)
{
	int	new_in;
	int	new_out;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	new_in = get_new_in(pipe_data, prev);
	new_out = get_new_out(pipe_data, pipe_fd);
	exec_child(pipe_data, new_in, new_out, pipe_fd);
	return ;
}
