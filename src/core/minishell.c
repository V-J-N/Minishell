/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 09:51:20 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell.c
 * @brief Contains the main Read-Evaluate-Print Loop (REPL) and core logic
 * for minishell execution flow.
 */
#include "minishell.h"

/**
 * @brief Checks the global signal status and converts it to a standard
 * exit code.
 * This function proceses the status set by the signal handler and resets the
 * global variable.
 * @param exit_code The previous exit status of the last command.
 * @return int The updated exit status.
 */
static int	sigint_check(int exit_code)
{
	if (g_exit_code == SIGINT || g_exit_code == 130)
	{
		g_exit_code = 0;
		return (130);
	}
	return (exit_code);
}

/**
 * @brief Reads the command line input from the user or a file descriptor.
 * Uses readline for interactive mode (TTY) to provide history and editing,
 * and get next line for non interactive mode.
 * @param interactive Flag indicating if the shell is running in interactive
 * mode.
 * @return char The input string, or NULL on EOF.
 */
static char	*read_input(int interactive)
{
	char	*input;

	if (interactive)
		return (readline("$> "));
	input = get_next_line(STDIN_FILENO);
	if (input && input[ft_strlen(input) - 1] == '\n')
	{
		input[ft_strlen(input) - 1] = '\0';
	}
	return (input);
}

/**
 * @brief Executes the Expander and Executor phases.
 * This function completes the Evaluate part of the REPL:
 * expansion, execution, and signal status update.
 * @param data The main data structure.
 * @param input The command line input string.
 * @param ex_code The current exit signal to pass to the expander.
 * @param inter Flag indicating interactive mode.
 * @return int The exit status of the executed command.
 */
static int	expand_and_execute(t_data *data, char *input, int ex_code, int inter)
{
	data->parsed->cmd_list = expander(data->parsed->cmd_list, \
	data->env, ex_code);
	ex_code = execute_all(data);
	if (inter)
		ex_code = sigint_check(ex_code);
	ft_cleanup_loop(data, input, 0);
	return (ex_code);
}

/**
 * @brief The core Read-Evaluate-Print Loop (REPL).
 * This loop continuously reads input, tokenizes, parses, expands, and
 * executes command until EOF or the exit builtin is encountered.
 * @param data The main data structure.
 * @param exit_code The initial or previous exit status.
 * @param input Pointer to the input string (used for readline or GNL).
 * @param inter Flag indicating interactive mode.
 * @return int The final exit status of the shell.
 */
static int	rep_loop(t_data *data, int exit_code, char *input, int inter)
{
	while (1)
	{
		input = read_input(inter);
		if (!input)
		{
			if (inter)
				printf("exit\n");
			break ;
		}
		if (*input)
		{
			if (inter)
				add_history(input);
			data->token = tokenizer(input);
			if (data->token)
				data->parsed = parse_command(&data->token);
			if (!data->token || !data->parsed)
			{
				ft_cleanup_loop(data, input, 1);
				continue ;
			}
			exit_code = expand_and_execute(data, input, exit_code, inter);
		}
	}
	return (exit_code);
}

/**
 * @brief Main entry point for the minishell program.
 * Initializes the environment, sets up signal handling, and starts the REPL.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param envp The environment array passed from the operating system.
 * @return int The final exit status of the shell.
 */
int	main(int argc, char **argv, char **envp)
{
	char			*input;
	int				exit_code;
	t_data			*data;
	int				interactive;

	(void)argc;
	(void)argv;
	if (argc != 1)
		ft_putstr_fd("Usage ./minishell\n", STDERR_FILENO);
	else
	{
		input = NULL;
		data = init_data(envp);
		if (!data)
			return (1);
		if (!shell_lvl_handler(data))
			return (1);
		setup_signals();
		exit_code = 0;
		interactive = isatty(STDIN_FILENO);
		exit_code = rep_loop(data, exit_code, input, interactive);
		ft_cleanup_end(data);
	}
	return (exit_code);
}
