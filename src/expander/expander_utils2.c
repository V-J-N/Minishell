/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:53:48 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 23:03:48 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_utils2.c
 * @brief Utility functions for handling specific cases of variable expansion,
 * particularly dealing with literal $ signs followed by sequences that should
 * be treated literally until a specific delimiter (likely a double quote).
 */
#include "minishell.h"

/**
 * @brief Appends the '$' character literally to the expanded argument value,
 * followed by subsequent characters until a double quote (ASCII 34) is found.
 * This function is typically used when a '$' is encountered inside double
 * quotes but is not followed by a valid variable name, or when the expansion
 * logic needs to be bypassed for a section.
 * @param arg The argument node (t_arg) being processed.
 * @param i The index currently pointing after the '$' that started the
 * sequence.
 * @return char* The updated expanded string (`arg->exp_value`).
 */
char	*append_literal(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = ft_calloc(2, sizeof(char));
	new[0] = '$';
	new[1] = '\0';
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	while (arg->value[i] != 34)
	{
		append_value(arg, i);
		i++;
	}
	return (arg->exp_value);
}

/**
 * @brief Appends the '$' character literally to the expanded filename,
 * followed by subsequent characters until a double quote (ASCII 34) is found.
 * This is the equivalent function for redirection filenames.
 * @param redir The redirection node (t_redir) being processed.
 * @param i The index currently pointing after the '$' that started the sequence.
 * @return char* The updated expanded filename string (`redir->exp_file`).
 */
char	*redir_literal(t_redir *redir, size_t i)
{
	char	*old;
	char	*new;

	old = redir->exp_file;
	new = ft_calloc(2, sizeof(char));
	new[0] = '$';
	new[1] = '\0';
	redir->exp_file = ft_strjoin(old, new);
	free(old);
	free(new);
	while (redir->file[i] != 34)
	{
		redir_value(redir, i);
		i++;
	}
	return (redir->exp_file);
}
