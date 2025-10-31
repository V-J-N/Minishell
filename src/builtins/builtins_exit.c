/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:09:17 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/31 23:03:19 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_exit.c
 * @brief Implementation of the 'exit' built-in command, handling argument
 * validation and comprehensive memory cleanup before terminating the shell.
 */
#include "minishell.h"

/**
 * @brief Checks if a string is a valid integer in bash-style.
 * This function validates the argument passed to `exit` to ensure it is
 * a numeric value, as required by POSIX shells.
 * @param str The string to check for numeric validity.
 * @return bool True if the string represents a valid integer, false otherwise.
 */
static bool	is_numeric(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

/**
 * @brief Central routine for freeing all dynamically allocated memory and
 * terminating the shell.
 * This function guarantees a clean exit by recursively freeing the command
 * pipeline, the environment, the main data structure, and Readline history.
 * @param exit_code The code to pass to the system's exit() function.
 * @param data A pointer to the main shell data structure.
 * @param args The temporary argument array (char **) created for this execution.
 * @note This function is the last call and will NOT return.
 */
static void	cleanup_and_exit(int exit_code, t_data *data, char **args)
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
	if (args)
		ft_free_array(args);
	rl_clear_history();
	exit(exit_code);
}

/**
 * @brief Handles the error case where the argument is not a valid number.
 * Prints the standard bash error message and immediately calls
 * `cleanup_and_exit` with a mandatory exit code of 2.
 * @param args The argument array (used to display the invalid argument).
 * @param data A pointer to the main shell data structure for cleanup.
 */
static void	no_numeric_arg(char **args, t_data *data)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	cleanup_and_exit(2, data, args);
}

/**
 * @brief Implements the `exit` built-in command.
 * Handles argument validation (numeric and argument count) and triggers
 * the complete cleanup process before terminating the shell.
 * @param data A pointer to the main shell data structure (`t_data`).
 * @param cmd_list The current 't_command' node containing the arguments.
 * @return int Returns EXIT_FAILURE (1) only if the shell should NOT exit due
 * to "too many arguments".
 * @note Because `exit` is state-modifying, it is expected to run in the
 * parent process.
 */
int	ft_exit(t_data *data, t_command *cmd_list)
{
	char	**args;
	long	code;

	if (!data || !data->parsed || !data->parsed->cmd_list \
	|| !data->parsed->cmd_list->args)
		cleanup_and_exit(EXIT_FAILURE, data, NULL);
	args = args_to_array(cmd_list->args);
	if (!args)
		cleanup_and_exit(EXIT_FAILURE, data, NULL);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args[1])
		cleanup_and_exit(0, data, args);
	if (!is_numeric(args[1]))
		no_numeric_arg(args, data);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		ft_free_array(args);
		return (EXIT_FAILURE);
	}
	code = ft_atol(args[1]);
	cleanup_and_exit((unsigned char)code, data, args);
	return (EXIT_SUCCESS);
}
