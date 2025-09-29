/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:09:17 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/29 06:09:47 by vjan-nie         ###   ########.fr       */
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

static void	no_numeric_arg(char **args)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	ft_free_array(args);
	exit(2);
}

/**
 * @brief Simulates the behavior of the bash `exit` builtin.
 * @return int Returns 1 if there is an error and shell should not exit.
 */
int	ft_exit(t_command *cmd_lst)
{
	char	**args;
	long	code;

	if (!cmd_lst || !cmd_lst->args)
		exit(0); // Just "exit" with no arguments
	args = args_to_array(cmd_lst->args);
	if (!args)
		exit(1);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
	{
		ft_free_array(args);
		exit(0);
	}
	if (!is_numeric(args[1]))
		no_numeric_arg(args);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		ft_free_array(args);
		return (1);
	}
	code = ft_atol(args[1]);
	ft_free_array(args);
	exit((unsigned char)code);
}
