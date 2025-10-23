/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:23:54 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/24 01:28:34 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell_utils.c
 * @brief Contains core utility functions for memory management and signal
 * handling.
 */
#include "minishell.h"

/**
 * @brief Frees a dynamically allocated NULL-terminated array of strings.
 * Iterates through the array, freeing each string pointer, and finally
 * frees the array pointer itself. Handles NULL input safely.
 * @param array The char** array to be freed.
 */
void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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

/**
 * @brief Sets up signal handling for the main shell process.
 * Configures the shell to use sigint_handler for SIGINT and ignores SIGQUIT
 * to prevent the shell from exiting and generating core dump files.
 */
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return ;
}

/**
 * Cleans up memory allocated during REPL execution loop.
 * Frees the current token list, the parsed command structure, and the input
 * line. Also prints a syntax error message if the error flag is set.
 * @param data The main data structure containing tokens and parsed commands.
 * @param input string read by readline.
 * @param error Boolean flag: true if a sintax error ocurred.
 */
void	ft_cleanup_loop(t_data *data, char *input, bool	error)
{
	if (data)
	{
		if (data->token)
			free_tokens(&data->token);
		if (data->parsed)
			free_parser(&data->parsed);
	}
	if (input)
		free(input);
	if (error)
		ft_putstr_fd("Syntax Error\n", STDERR_FILENO);
	return ;
}

/**
 * @brief Performs final memory cleanup before the shell exits.
 * Frees all major components: tokens, parsed commands, the environment list,
 * the main data structure itself, and clears the readline history.
 * @param data The main data structure to be freed.
 */
void	ft_cleanup_end(t_data *data)
{
	if (data)
	{
		if (data->token)
			free_tokens(&data->token);
		if (data->parsed)
			free_parser(&data->parsed);
		if (data->env)
			free_environment(&data->env);
		free(data);
	}
	rl_clear_history();
	return ;
}
