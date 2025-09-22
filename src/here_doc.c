/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:11:34 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/22 14:33:50 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	prepare_all_heredocs(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (!prepare_heredocs(current))
			return (false);
		current = current->next;
	}
	return (true);
}

/** @brief Handles heredoc through a pipe 
 * and a child process, before any execution.
 */
bool	prepare_heredocs(t_command *cmd)
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
			fd = get_heredoc_fd(redir->file);
			if (fd == -1)
				return (false);
			redir->heredoc_fd = fd;
		}
		redir = redir->next;
	}
	return (true);
}

static bool	is_limiter(char *limiter, char *line)
{
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 \
	&& line[ft_strlen(limiter)] == '\n')
		return (true);
	else
		return (false);
}

static void	heredoc_child_process(char *limiter, int *pipe_fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[0]);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			exit(EXIT_SUCCESS);
		if (is_limiter(limiter, line))
		{
			free(line);
			break ;
		}
		if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
		{
			perror("write to pipe");
			free(line);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

/** @brief Heredoc is handled through a child process in which
 * the STDIN is piped to a loop that takes user input until EOF,
 * returning the reading end of the pipe. SIGINT is ignored in the
 * parent process until child process ends (waitpid).
 */
int	get_heredoc_fd(char *limiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (perror("here_doc pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		heredoc_child_process(limiter, pipe_fd);
	signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipe_fd[0]), -1);
	return (pipe_fd[0]);
}
