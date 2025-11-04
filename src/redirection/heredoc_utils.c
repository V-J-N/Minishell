/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 08:42:54 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/04 01:46:03 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file here_doc_utils.c
 * @brief Utility functions for processing heredocs (<<), including opening
 * FDs, managing the child process, and handling signals during input reading.
 */
#include "minishell.h"

/**
 * @brief Iterates over the entire list of commands and prepares all
 * associated heredocs before execution begins.
 * It ensures all heredocs are read and stored in temporary FDs. If any heredoc
 * fails (e.g., is interrupted by SIGINT), the execution is aborted.
 * @param cmd_list The head of the command list (t_command *).
 * @param data Pointer to the main data structure.
 * @param excode The exit status of the previous command ($?).
 * @return bool Returns true if all heredocs were successfully prepared,
 * false otherwise.
 */
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

/**
 * @brief Processes all redirections of type HEREDOC for a single command node.
 * * For each HEREDOC, it calls `get_heredoc_fd` (assumed to handle the pipe/fork
 * and input reading) and stores the resulting read file descriptor (`fd`) in the
 * redirection node structure.
 * @param cmd The command node being checked.
 * @param data Pointer to the main data structure.
 * @param excode The exit status of the previous command ($?).
 * @return bool Returns true if all heredocs were processed successfully, false
 * on error.
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

/**
 * @brief Signal handler specifically used when the shell is reading input
 * for a heredoc inside a child process.
 * Upon receiving SIGINT, it sets the global exit code, prints a newline, closes
 * STDIN, and exits the child process with status 130 (128 + 2 for SIGINT).
 * @param sign The signal number (expected to be SIGINT).
 */
void	heredoc_sigint_handler(int sign)
{
	g_exit_code = sign;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	exit(130);
}

/**
 * @brief Checks the exit status of the child process that was reading the
 * heredoc input.
 * If the child process exited due to SIGINT (status 130 or signaled SIGINT),
 * it cleans up the pipe read end and reports an error (-1) to abort execution.
 * @param status The status returned by `waitpid` on the heredoc child process.
 * @param pipe_fd_readend The read end file descriptor of the pipe holding the
 * heredoc content.
 * @return int Returns the read end FD if successful, or -1 if the heredoc was
 * interrupted.
 */
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
