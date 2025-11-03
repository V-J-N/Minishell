/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:11:34 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/03 11:42:30 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_write_pipe(char *line, int *pipe_fd)
{
	if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
	{
		perror("write to pipe");
		exit(EXIT_FAILURE);
	}
	return ;
}

static void	process_line(t_redir *redir, char **line, t_data *data, int excode)
{
	char	*expanded;

	expanded = NULL;
	if (!redir->has_quotes)
	{
		expanded = expand_heredoc_line(*line, data->env, excode);
		if (expanded)
		{
			free(*line);
			*line = expanded;
		}
	}
	return ;
}

static void	user_loop(t_redir *redir, int *pipe_fd, t_data *data, int excode)
{
	char	*line;
	char	*true_limiter;
	int		stop;

	true_limiter = ft_strdup(redir->file);
	stop = 0;
	while (!stop)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			heredoc_canceled(true_limiter, pipe_fd);
		if (is_limiter(true_limiter, line))
			stop = 1;
		else
		{
			process_line(redir, &line, data, excode);
			heredoc_write_pipe(line, pipe_fd);
		}
		free(line);
	}
	free(true_limiter);
	return ;
}

static void	hd_ch_process(t_redir *redir, int *pipe_fd, t_data *dt, int excode)
{
	signal(SIGINT, heredoc_sigint_handler);
	close(pipe_fd[0]);
	user_loop(redir, pipe_fd, dt, excode);
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
		hd_ch_process(redir, pipe_fd, data, excode);
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	return (heredoc_end(status, pipe_fd[0]));
}
