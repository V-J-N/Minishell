/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:23:11 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/31 22:19:25 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_echo.c
 * @brief Implementation of the 'echo' built-in command, supporting
 * the '-n' flag.
 */
#include "minishell.h"

/**
 * @brief Iteratively prints the command arguments to standard output (STDOUT).
 * Arguments are printed separated by a single space.
 * @param args The head of the 't_arg' list containing the strings to print,
 * starting after the command name and any leading '-n' flags.
 */
static void	echo_print(t_arg *args)
{
	while (args)
	{
		ft_printf("%s", args->value);
		if (args->next)
			ft_printf(" ");
		args = args->next;
	}
	return ;
}

/**
 * @brief Implements the 'echo' built-in command logic.
 * This function handles argument parsing, specifically checking for the '-n'
 * option (or `-nnnnn...` variants) before printing the remaining arguments.
 * @param cmd A pointer to the 't_command' node containing the arguments list.
 * @return int Returns EXIT_SUCCESS (0) on completion. Returns EXIT_FAILURE (1)
 * only if the command node is invalid or the command name isn't 'echo'.
 */
int	ft_echo(t_command *cmd)
{
	bool	newline;
	t_arg	*temp;
	int		i;

	if (!cmd || ft_strncmp(cmd->args->value, "echo", 5))
		return (EXIT_FAILURE);
	temp = cmd->args->next;
	newline = true;
	while (temp && temp->value && ft_strncmp(temp->value, "-n", 2) == 0)
	{
		i = 2;
		while (temp->value[i] == 'n')
			i++;
		if (temp->value[i] != '\0')
			break ;
		newline = false;
		temp = temp->next;
	}
	echo_print(temp);
	if (newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
