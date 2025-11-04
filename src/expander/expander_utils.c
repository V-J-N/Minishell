/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:26:41 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 18:08:52 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_utils.c
 * @brief Utility functions for the variable expansion process, including
 * string building, environment value retrieval, and exit code substitution.
 */
#include "minishell.h"

/**
 * @brief Checks if a given string contains the dollar sign ('$').
 * This is used to optimize the expansion process, skipping the full
 * character-by-character check if the dollar sign isn't present in the
 * token's value.
 * @param str The string to inspect.
 * @return bool True if a '$' (ASCII 36) is found, false otherwise.
 */
bool	contains_dollar(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 36)
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief Appends a single character from the original argument value to the
 * temporary expanded string (`arg->exp_value`).
 * This function is used to handle literal characters that are not part of a
 * variable expansion.
 * @param arg The argument node being processed.
 * @param i The index of the character in `arg->value` to be appended.
 * @return char* The updated `arg->env_value`.
 * @note This function performs dynamic memory allocation and concatenation.
 */
char	*append_value(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = ft_calloc(2, sizeof(char));
	if (!new)
		return (NULL);
	new[0] = arg->value[i];
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	arg->i++;
	return (arg->env_value);
}

/**
 * @brief Appends a single character from the original argument value to the
 * temporary variable name being collected (`arg->env_value`).
 * This function is used exclusively to build the variable key ("USER", "HOME")
 * right after a '$' is encountered.
 * @param arg The argument node being processed.
 * @param i The index of the character in `arg->value` to be appended.
 * @return char* The updated `arg->env_value` (the variable name being built).
 */
char	*append_environment(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->env_value;
	new = ft_calloc(2, sizeof(char));
	if (!new)
		return (NULL);
	new[0] = arg->value[i];
	arg->env_value = ft_strjoin(old, new);
	free(old);
	free(new);
	arg->i++;
	return (arg->env_value);
}

/**
 * @brief Retrieves the value associated with the built-up variable name and
 * appends it to the final expanded string (`arg->exp_value`).
 * This completes the substitution step for a single environment variable.
 * @param arg The argument node (containing `arg->env_value`).
 * @param env The head of the environment list.
 * @return char* The updated `arg->exp_value`.
 */
char	*get_expand_env(t_arg *arg, t_env *env)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = get_value_by_key(env, arg->env_value);
	if (!new)
		new = ft_strdup("");
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	free(arg->env_value);
	arg->env_value = NULL;
	return (arg->exp_value);
}

/**
 * @brief Appends the string representation of the last exit code ($?) to the
 * final expanded string (`arg->exp_value`).
 * @param arg The argument node being processed.
 * @param exit_code The status code (integer) of the last command.
 * @return char* The updated `arg->exp_value`.
 */
char	*append_exit(t_arg *arg, int exit_code)
{
	char	*old;
	char	*new;
	char	*itoa_value;

	old = arg->exp_value;
	itoa_value = ft_itoa(exit_code);
	new = ft_strjoin(old, itoa_value);
	if (!new)
		return (NULL);
	free(old);
	free(itoa_value);
	arg->exp_value = new;
	arg->i++;
	return (arg->exp_value);
}
