/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset_pwd_env.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:05:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/06 13:06:27 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Prints PWD from env_list.
 * @warning No sé cómo debería actuar si PWD
 * está corrupto. Uso getcwd como backup pero
 * no sé si debería dejarlo romperse en paz.
 */
int	ft_pwd(t_env *env)
{
	char	*value;
	char	*cwd;

	if (!env)
		return (perror("env error"), EXIT_FAILURE);
	value = get_value_by_key(env, "PWD");
	if (value && *value && access(value, F_OK) == 0)
	{
		ft_printf("%s\n", value);
		free(value);
		return (EXIT_SUCCESS);
	}
	free(value);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("getcwd error"), EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}

/** @brief Shows in terminal the environment variables
 * stored in the t_env list.
 */
int	ft_env(t_env *env, t_command *cmd_lst)
{
	t_env	*temp;

	if (!env)
	{
		ft_putstr_fd("env error\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd_lst->cmd_argc > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	temp = env;
	while (temp)
	{
		ft_printf("%s\n", temp->full_env);
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Removes environment variables based on given keys.
 */
int	ft_unset(t_command *cmd_lst, t_env **env)
{
	char	**args;
	int		i;

	if (!cmd_lst || !cmd_lst->args || !env)
		return (1);
	args = args_to_array(cmd_lst->args);
	if (!args)
		return (1);
	i = 1;
	while (args[i])
	{
		delete_env_key(env, args[i]);
		i++;
	}
	ft_free_array(args);
	return (0);
}
