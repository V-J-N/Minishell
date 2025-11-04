/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_args2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 12:25:56 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 18:01:20 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_args2.c
 * @brief Detailed logic for expanding environment variables and the exit
 * status ($?) within command arguments.
 */
#include "minishell.h"

/**
 * @brief Handles the content found after a '$' sign and appends the expanded
 * value (or the original character) to the temporary expanded string.
 * It manages special cases like `$?` (exit status) and variable names.
 * The index `arg->i` is advanced during the process.
 * @param arg The current argument node being processed.
 * @param env The head of the environment list.
 * @param exit The status code of the last executed command ($?).
 */
static void	append_dollar(t_arg *arg, t_env *env, int exit)
{
	if (arg->value[arg->i + 1] == '\0')
	{
		append_value(arg, arg->i);
		return ;
	}
	if (arg->value[arg->i] == 36)
	{
		arg->i++;
		if (ft_isdigit(arg->value[arg->i]))
			arg->i++;
		else if (arg->value[arg->i] == 63)
			append_exit(arg, exit);
		else if (ft_isalnum(arg->value[arg->i]) || arg->value[arg->i] == '_')
		{
			arg->env_value = ft_strdup("");
			if (!arg->env_value)
				return ;
			while (ft_isalnum(arg->value[arg->i]) || arg->value[arg->i] == '_')
				append_environment(arg, arg->i);
			if (arg->env_value)
				get_expand_env(arg, env);
		}
	}
	else
		append_value(arg, arg->i);
}

/**
 * @brief Iterates through all arguments of a command and performs variable
 * expansion for those marked for it.
 * Expansion applies to:
 * - Arguments not contained in single quotes.
 * - Variables of the form $VAR, $?, and $1 (ignored but parsed).
 * @param args The head of the 't_arg' list for a command.
 * @param env The head of the environment list.
 * @param exit The status code of the last executed command ($?).
 * @return t_arg* Returns the head of the command list (modified in place)
 * or NULL on memory failure.
 */
t_arg	*expand_args(t_arg *args, t_env *env, int exit)
{
	t_arg	*temp;

	temp = args;
	while (temp)
	{
		if (temp->is_expanded && contains_dollar(temp->value))
		{
			if (temp->value[temp->i + 1] == '\0')
			{
				temp = temp->next;
				continue ;
			}
			temp->exp_value = ft_strdup("");
			if (!temp->exp_value)
				return (NULL);
			while (temp->value[temp->i])
				append_dollar(temp, env, exit);
			temp->is_expanded = false;
			if (temp->exp_value)
				change_value(temp);
		}
		temp = temp->next;
	}
	return (temp);
}
