/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:11:34 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 20:27:22 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_limiter(char *limiter, char *line)
{
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 \
	&& line[ft_strlen(limiter)] == '\n')
		return (true);
	else
		return (false);
}

static void	heredoc_write_pipe(char *line, int *pipe_fd)
{
	if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
	{
		perror("write to pipe");
		free(line);
		exit(EXIT_FAILURE);
	}
	free(line);
	return ;
	
}

static void	heredoc_user_loop(t_redir *redir, int *pipe_fd, t_data *data, int excode)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			exit(EXIT_SUCCESS);
		if (redir->is_expanded)
		{
			expanded = expand_heredoc_line(line, data->env, excode);
			if (expanded)
			{
				free(line);
				line = expanded;
			}
		}
		if (is_limiter(redir->file, line))
		{
			free(line);
			break ;
		}
		heredoc_write_pipe(line, pipe_fd);
	}
}

static void	heredoc_child_process(t_redir *redir, int *pipe_fd, t_data *data, int excode)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, heredoc_sigint_handler);
	close(pipe_fd[0]);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			exit(EXIT_SUCCESS);
		if (redir->is_expanded)
		{
			expanded = expand_heredoc_line(line, data->env, excode);
			if (expanded)
			{
				free(line);
				line = expanded;
			}
		}
		if (is_limiter(redir->file, line))
		{
			free(line);
			break ;
		}
		heredoc_write_pipe(line, pipe_fd);
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
int	get_heredoc_fd(t_redir *redir, t_data *data, int excode)
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
		heredoc_child_process(redir, pipe_fd, data, excode);
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	return (heredoc_end(status, pipe_fd[0]));
}
