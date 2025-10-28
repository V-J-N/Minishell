/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 08:42:54 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/28 10:46:30 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
