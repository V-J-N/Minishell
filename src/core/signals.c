/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 07:54:20 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 09:10:05 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

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
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
	return ;
}


/* void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return ;
} */
