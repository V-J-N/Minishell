/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/24 13:47:09 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell_utils2.c
 * @brief Contains signal handling utilities for the main shell process.
 * This file provides robust siganl configuration using sigaction to handle
 * interactive interruptions and to record termination signals.
 */
#include "minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * This handler is active when the shell is waiting for user input.
 * It updates the global status, writes a newline, and resets the
 * readline prompt.
 * @param signum The signal number received.
 */
static void	sigint_handler(int signum)
{
	g_sigint_status = signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}

/**
 * @brief Signahl handler for SIGQUIT (Ctrl+\).
 * This handler is active when the shell is waiting for user input. It
 * only records signal status but does not print anything of affect the
 * readline prompt, as is standard behavior for the main shell loop.
 * @param signum The signal number received.
 */
static void	sigquit_handler(int signum)
{
	g_sigint_status = signum;
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
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
	return ;
}
