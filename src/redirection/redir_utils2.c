/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:00:54 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/06 13:09:06 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_redirs(t_command *cmd)
{
	return (cmd && cmd->redirs != NULL);
}

int	manage_out_redir(t_redir *redir, int *fd, int *last_fd)
{
	if (*fd != STDOUT_FILENO)
		close(*fd);
	if (redir->type == REDIR_OUT)
		*fd = get_outputfile_fd(redir->file);
	else
		*fd = get_append_fd(redir->file);
	if (*fd == -1)
	{
		if (*last_fd != -1 && *last_fd != *fd)
			close(*last_fd);
		return (-1);
	}
	if (*last_fd != -1 && *last_fd != *fd)
		close(*last_fd);
	*last_fd = *fd;
	return (0);
}

int	manage_in_redir(char *file, int *fd, int *last_fd)
{
	if (*fd != STDIN_FILENO)
		close(*fd);
	*fd = get_inputfile_fd(file);
	if (*fd == -1)
	{
		if (*last_fd != -1 && *last_fd != *fd)
			close(*last_fd);
		return (-1);
	}
	if (*last_fd != -1 && *last_fd != *fd)
		close(*last_fd);
	*last_fd = *fd;
	return (0);
}

int	manage_heredoc(int heredoc_fd, int *fd, int *last_fd)
{
	if (*fd != STDIN_FILENO)
		close(*fd);
	*fd = heredoc_fd;
	if (*last_fd != -1 && *last_fd != *fd)
		close(*last_fd);
	*last_fd = *fd;
	return (0);
}
