/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 07:54:20 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/28 10:42:19 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks the global signal status and converts it to a standard
 * exit code.
 * This function proceses the status set by the signal handler and resets the
 * global variable.
 * @param exit_code The previous exit status of the last command.
 * @return int The updated exit status.
 */
int	sigint_check(int exit_code)
{
	if (g_exit_code == SIGINT || g_exit_code == 130)
	{
		g_exit_code = 0;
		return (130);
	}
	return (exit_code);
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * This handler is active when the shell is waiting for user input.
 * It updates the global status, writes a newline, and resets the
 * readline prompt.
 * @param signum The signal number received.
 */
void	sigint_handler(int signum)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = signum;
	return ;
}

/**
 * @brief Sets up robust signal handling using sigaction.
 * Configures the siganl actions for SIGINT and SIGQUIT.
 * - SIGINT is handled to refresh the prompt.
 * - SIGQUIT is handled to record the status.
 * The SA_RESTART flag is used automatically restart interrupted system
 * calls.
 */
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return ;
}
