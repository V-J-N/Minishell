/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:23:54 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/24 19:52:47 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell_utils.c
 * @brief Contains core utility functions for memory management and signal
 * handling.
 */
#include "minishell.h"

/**
 * @brief Initializes the main minishell data structure (t_data).
 * Allocates memory for the t_data structure and initializes its core components
 * to NULL. It then populates the internal environment list from the system's
 * environment array.
 * @param envp The environment array passed to main.
 * @return t_data A pointer to the newly initialized data structure, or NULL
 * on failure.
 */
t_data	*init_data(char **envp)
{
	t_data			*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = NULL;
	data->token = NULL;
	data->parsed = NULL;
	if (!get_environment(envp, &data->env))
	{
		ft_putstr_fd("Minishell: envp copy failed\n", STDERR_FILENO);
		free(data);
		return (NULL);
	}
	return (data);
}

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
