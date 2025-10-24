/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:08:07 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/24 12:12:21 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	no_shell_lvl(t_data *data)
{
	t_env	*new;

	new = ft_lstnew_mini_env("SHLVL", "1", "SHLVL=1");
	if (!new)
		return (false);
	ft_addback_mini_env(&data->env, new);
	return (true);
}

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
