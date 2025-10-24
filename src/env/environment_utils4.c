/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:08:07 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/25 00:10:35 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file environment_utils4.c
 * @brief Utilities dedicated to managing the SHLVL environment variable.
 * This file contains the logic for finding, updating, and handling edge cases
 * related to the SHLVL variable, which tracks the nesting level of the shell.
 */
static bool	no_shell_lvl(t_data *data)
{
	t_env	*new;

	new = ft_lstnew_mini_env("SHLVL", "1", "SHLVL=1");
	if (!new)
		return (false);
	ft_addback_mini_env(&data->env, new);
	return (true);
}

/**
 * @brief Handles the case where the SHLVL variable is not present in
 * the environment.
 * It creates a new t_env node for SHLVL=1 and adds it to the environment list.
 * @param data The main data structure containing the environment list.
 * @return true On successful creation and addition of SHLVL=1.
 * @return false If memory allocation for the new node fails.
 */
static bool	update_shell_lvl(t_env *shl_lvl, char *str_value)
{
	char	*full_value;

	full_value = ft_strjoin("SHLVL=", str_value);
	if (!full_value)
		return (free(str_value), false);
	free(shl_lvl->value);
	free(shl_lvl->full_env);
	shl_lvl->value = str_value;
	shl_lvl->full_env = full_value;
	return (true);
}

/**
 * @brief Updates the key, value, and full_env strings of an existing
 * SHLVL node.
 * This function handles the memory management for updating the variable's
 * value after it has been incremented. It frees the old value/full_env and
 * allocates the new full_env string.
 * @param shl_lvl A pointer to the existing 't_env' node for SHLVL.
 * @param str_value The new, dynamically allocated string representation
 * of the incremented level.
 * @return true On successful update.
 * @return false If memory allocation for the new full_env string fails.
 */
bool	shell_lvl_handler(t_data *data)
{
	t_env	*shl_lvl;
	char	*str_value;
	int		int_value;

	if (!data || !data->env)
		return (false);
	shl_lvl = find_node_by_key(data->env, "SHLVL");
	if (!shl_lvl)
		return (no_shell_lvl(data));
	int_value = ft_atoi(shl_lvl->value);
	if (int_value >= SHLVL_MAX)
	{
		ft_putstr_fd("Minishell SHLVL too high\n", STDERR_FILENO);
		ft_cleanup_end(data);
		exit(1);
	}
	if (int_value < 1)
		int_value = 1;
	else
		int_value ++;
	str_value = ft_itoa(int_value);
	if (!str_value)
		return (false);
	return (update_shell_lvl(shl_lvl, str_value));
}
