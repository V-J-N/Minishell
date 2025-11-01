/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 08:42:54 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 20:14:02 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	prepare_all_heredocs(t_command *cmd_list, t_data *data, int excode)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (!prepare_heredocs(current, data, excode))
			return (false);
		current = current->next;
	}
	return (true);
}

/** @brief Handles heredoc through a pipe 
 * and a child process, before any execution.
 */
bool	prepare_heredocs(t_command *cmd, t_data *data, int excode)
{
	t_redir	*redir;
	int		fd;

	if (!cmd)
		return (true);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			fd = get_heredoc_fd(redir, data, excode);
			if (fd == -1)
				return (false);
			redir->heredoc_fd = fd;
		}
		redir = redir->next;
	}
	return (true);
}

void	heredoc_sigint_handler(int sign)
{
	g_exit_code = sign;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	exit(130);
}

int	heredoc_end(int status, int pipe_fd_readend)
{
	int	code;

	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (code == 130)
		{
			g_exit_code = 130;
			close(pipe_fd_readend);
			return (-1);
		}
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_exit_code = 130;
		close(pipe_fd_readend);
		return (-1);
	}
	return (pipe_fd_readend);
}
