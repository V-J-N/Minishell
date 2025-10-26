/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:09:17 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 11:44:59 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a string is a valid integer in bash-style.
 */
static bool	is_numeric(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

static void	cleanup_and_exit(int exit_code, t_data *data, char **args)
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
	if (args)
		ft_free_array(args);
	rl_clear_history();
	exit(exit_code);
}

static void	no_numeric_arg(char **args, t_data *data)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	cleanup_and_exit(2, data, args);
}

/**
 * @brief Simulates the behavior of the bash `exit` builtin.
 * @return int Returns 1 if there is an error and shell should not exit.
 */
int	ft_exit(t_data *data, t_command *cmd_list)
{
	char	**args;
	long	code;

	if (!data || !data->parsed || !data->parsed->cmd_list \
	|| !data->parsed->cmd_list->args)
		cleanup_and_exit(0, data, NULL);
	args = args_to_array(cmd_list->args);
	if (!args)
		cleanup_and_exit(1, data, NULL);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
		cleanup_and_exit(0, data, args);
	if (!is_numeric(args[1]))
		no_numeric_arg(args, data);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		ft_free_array(args);
		return (1);
	}
	code = ft_atol(args[1]);
	cleanup_and_exit((unsigned char)code, data, args);
	return (0);
}
