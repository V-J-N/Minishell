/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:24:54 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 13:37:41 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander.c
 * @brief Handles the expansion of environment variables and the removal of
 * quotes within command arguments and redirection filenames.
 */
#include "minishell.h"

/**
 * @brief Scans a string to determine if it contains single or double quotes.
 * This function is typically used as a second check to confirm the type of
 * quotes present in a token if the initial lexer/parser marked it as having
 * quotes.
 * It returns the type of the first quote found.
 * @param str The string (argument or filename) to check.
 * @return t_token_quote The type of quote found (SINGLE, DOUBLE), or NONE.
 * @note ASCII 39 is single quote ('), ASCII 34 is double quote (").
 */
static t_token_quote	check_has_quotes(char *str)
{
	size_t			i;
	t_token_quote	quote;

	i = 0;
	quote = NONE;
	while (str[i])
	{
		if (str[i] == 39)
			return (quote = SINGLE);
		else if (str[i] == 34)
			return (quote = DOUBLE);
		i++;
	}
	return (quote);
}

/**
 * @brief Processes the argument list of a single command node after
 * initial expansion.
 * This function iterates through the arguments, re-checks quote type if
 * necessary, applies quote removal and updates the argument value if
 * expansion produced a result (`exp_value`).
 * @param args The head of the 't_arg' list for a command.
 * @return t_arg* Returns NULL after processing.
 */
static t_arg	*check_arguments(t_arg *args)
{
	t_arg	*temp;

	temp = args;
	while (temp)
	{
		if (temp->quote == NONE && temp->has_quotes == true)
			temp->quote = check_has_quotes(temp->value);
		if (temp->has_quotes)
		{
			if (temp->quote == SINGLE)
				check_singles_arg(temp);
			else if (temp->quote == DOUBLE)
				check_doubles_arg(temp);
		}
		if (temp->exp_value != NULL)
			change_value(temp);
		temp = temp->next;
	}
	return (temp);
}

/**
 * @brief Processes the redirection list of a single command node after
 * initial expansion.
 * This function performs the same quote handling and value updating
 * as `check_arguments` but specifically for redirection filenames.
 * @param redirs The head of the 't_redir' list for a command.
 * @return t_redir* Returns NULL after processing.
 */
static t_redir	*check_redirs(t_redir *redirs)
{
	t_redir	*temp;

	temp = redirs;
	while (temp)
	{
		if (temp->quote == NONE && temp->has_quotes == true)
			temp->quote = check_has_quotes(temp->file);
		if (temp->has_quotes)
		{
			if (temp->quote == SINGLE)
				check_singles_redir(temp);
			if (temp->quote == DOUBLE)
				check_doubles_redir(temp);
		}
		if (temp->exp_file != NULL)
			change_file(temp);
		temp = temp->next;
	}
	return (temp);
}

/**
 * @brief Main function to perform environment variable expansion and
 * quote removal across the entire command pipeline.
 * This function orchestrates the two main expansion phases:
 * 1. Variable expansion (`expand_args`/`expand_redirs`).
 * 2. Quote removal/final value update (`check_arguments`/`check_redirs`).
 * @param cmd_list The head of the command pipeline (t_command*).
 * @param env The head of the environment list (t_env*).
 * @param exit_signal The status code of the last executed command ($?).
 * @return t_command* Returns the head of the command list.
 */
t_command	*expander(t_command *cmd_list, t_env *env, int exit_signal)
{
	t_command	*tmp;
	t_arg		*temp;
	t_redir		*temp2;

	tmp = cmd_list;
	while (tmp)
	{
		temp = tmp->args;
		expand_args(temp, env, exit_signal);
		temp2 = tmp->redirs;
		expand_redirs(temp2, env, exit_signal);
		temp = tmp->args;
		check_arguments(temp);
		temp2 = tmp->redirs;
		check_redirs(temp2);
		tmp = tmp->next;
	}
	return (cmd_list);
}
