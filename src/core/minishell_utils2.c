/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/24 07:33:07 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell_utils2.c
 * @brief Contains initialization utilities for the main shell data structure.
 */
#include "minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * Sets the global status flag, writes a newline, and updates the readline
 * prompt to maintain proper terminal state after and interrupt.
 * @param signum The signal number received.
 */
void	sigint_handler(int signum)
{
	g_sigint_status = signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}

void	sigquit_handler(int signum)
{
	g_sigint_status = signum;
	return ;
}

/**
 * @brief Sets up signal handling for the main shell process.
 * Configures the shell to use sigint_handler for SIGINT and ignores SIGQUIT
 * to prevent the shell from exiting and generating core dump files.
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
	return ;
}
