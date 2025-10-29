/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/29 12:42:56 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell.c
 * @brief Contains the main Read-Evaluate-Print Loop (REPL) and core logic
 * for minishell execution flow.
 */
#include "minishell.h"

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
	if (input && ft_strlen(input) > 0 && input[ft_strlen(input) - 1] == '\n')
		input[ft_strlen(input) - 1] = '\0';
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
static int	expand_and_execute(t_data *data, char *input, int excode, int inter)
{
	data->parsed->cmd_list = expander(data->parsed->cmd_list, \
	data->env, excode);
	excode = execute_all(data);
	if (inter)
		excode = sigint_check(excode);
	ft_cleanup_loop(data, input, 0);
	return (excode);
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
			break ;
		if (*input)
		{
			if (inter)
			{
				exit_code = sigint_check(exit_code);
				add_history(input);
			}
			data->token = tokenizer(input);
			if (data->token)
				data->parsed = parse_command(&data->token);
			if (!data->token || !data->parsed)
				ft_cleanup_loop(data, input, 1);
			else
				exit_code = expand_and_execute(data, input, exit_code, inter);
		}
		else
			free(input);
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
	exit_code = 0;
	if (argc != 1)
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
	input = NULL;
	data = init_data(envp);
	if (!data)
		return (1);
	if (!shell_lvl_handler(data))
		return (1);
	setup_signals();
	interactive = isatty(STDIN_FILENO);
	exit_code = rep_loop(data, exit_code, input, interactive);
	if (interactive)
		ft_putstr_fd("exit\n", STDIN_FILENO);
	ft_cleanup_end(data);
	return (exit_code);
}
