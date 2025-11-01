/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 20:32:04 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 09:20:38 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_export_utils.c
 * @brief Utility functions for the 'export' built-in, including identifier
 * validation and environment list printing in 'export -p' format.
 */
#include "minishell.h"

/**
 * @brief Checks if a potential environment variable name is a valid identifier.
 * A valid identifier must adhere to POSIX shell standards:
 * 1 - Must start with an underscore ('_') or an alphabetic character.
 * 2 - Subsequent characters must be alphanumeric or an underscore.
 * 3 - Validation stops at the first '=' sign (if present).
 * @param var The argument string being checked.
 * @return bool True if the key part is a valid identifier, false otherwise.
 */
bool	is_valid_identifier(const char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (false);
	i = 0;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Prints the environment list in the format required by `export`.
 * This function iterates through the `t_env` list and prints each variable with
 * the `declare -x` prefix. Variables without a value are printed
 * without quotes.
 * @param env The head of the environment list (t_env*).
 * @note This function assumes the list is already sorted, or that sorting
 * is handled by a caller function before passing the list here.
 */
void	print_sorted_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			ft_printf("declare -x %s\n", env->key);
		env = env->next;
	}
	return ;
}

/**
 * @brief Prints the standard bash error message for an invalid identifier
 * in `export`.
 * Format: `minishell: export: 'invalid_var': not a valid identifier`
 * @param var The invalid argument string that caused the error.
 */
void	ft_not_valid(char *var)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(var, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return ;
}
