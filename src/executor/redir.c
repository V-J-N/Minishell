/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:50:20 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/06 13:08:32 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Iterates through redirs in the command_list
 * and opens FDs according to files to be used as input
 * instead of STDIN. If there are multiple redirs, the 
 * last one prevails closing the previous ones, as in bash.
 */
int	redirect_in(t_command *cmd, int in_fd)
{
	t_redir	*temp;
	int		fd;
	int		last_fd;

	fd = in_fd;
	last_fd = -1;
	temp = cmd->redirs;
	while (temp)
	{
		if (temp->type == REDIR_IN)
		{
			if (manage_in_redir(temp->file, &fd, &last_fd) == -1)
				return (-1);
		}
		else if (temp->type == HEREDOC)
		{
			if (manage_heredoc(temp->heredoc_fd, &fd, &last_fd) == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd);
}

/** @brief Iterates through redirs in the command_list
 * and opens FDs according to files to be used as output
 * instead of STDOUT. If there are multiple redirs, the 
 * last one prevails closing the previous ones, as in bash.
 */
int	redirect_out(t_command *cmd, int out_fd)
{
	t_redir	*temp;
	int		fd;
	int		last_fd;

	fd = out_fd;
	last_fd = -1;
	temp = cmd->redirs;
	while (temp)
	{
		if (temp->type == REDIR_OUT || temp->type == APPEND)
		{
			if (manage_out_redir(temp, &fd, &last_fd) == -1)
				return (-1);
		}
		temp = temp->next;
	}
	return (fd);
}

static void	redirection_only_child_process(t_command *cmd, int in, int out)
{
	int	new_in;
	int	new_out;

	signal(SIGINT, SIG_DFL);
	new_in = redirect_in(cmd, in);
	new_out = redirect_out(cmd, out);
	if (new_in == -1 || new_out == -1)
		exit(EXIT_FAILURE);
	if (new_in != STDIN_FILENO)
	{
		dup2(new_in, STDIN_FILENO);
		close(new_in);
	}
	if (new_out != STDOUT_FILENO)
	{
		dup2(new_out, STDOUT_FILENO);
		close(new_out);
	}
	exit(EXIT_SUCCESS);
}

/** @brief Just prepares FD in a child process,
 * but doesn't execute any command.
 */
int	redirection_only(t_command *cmd, int in, int out)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("Fork"), EXIT_FAILURE);
	if (pid == 0)
		redirection_only_child_process(cmd, in, out);
	signal(SIGINT, SIG_IGN);
	status = ft_wait_and_exit(pid);
	signal(SIGINT, SIG_DFL);
	return (status);
}
