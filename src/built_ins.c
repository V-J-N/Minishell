/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/15 14:23:52 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *cmd)
{
	pid_t	pid;
	int		exit_return;

	if (ft_strncmp(cmd, "cd\0", 3) || ft_strncmp(cmd, "export\0", 7) || \
	ft_strncmp(cmd, "unset\0", 6) || ft_strncmp(cmd, "exit\0", 5))
		return (execute_built_in(cmd));
	if (ft_strncmp(cmd, "echo\0", 5) || ft_strncmp(cmd, "pwd\0", 4) || \
	ft_strncmp(cmd, "env\0", 4))
	{
		pid = fork();
		if (pid == 0)
		{
			exit_return = execute_built_in(cmd);
			if (exit_return == -1)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);
		}
		else
			return (ft_wait_and_exit(pid));
	}
	return (-1);
}

int	execute_built_in(char *cmd)
{
	if (ft_strncmp(cmd, "cd\0", 3))
		return (ft_cd());
	else if (ft_strncmp(cmd, "export\0", 7))
		return (ft_export());
	else if (ft_strncmp(cmd, "unset\0", 6))
		return (ft_unset());
	else if (ft_strncmp(cmd, "exit\0", 5))
		return (ft_exit());
	else if (ft_strncmp(cmd, "echo\0", 5))
		return (ft_echo());
	else if (ft_strncmp(cmd, "pwd\0", 4))
		return (ft_pwd());
	else if (ft_strncmp(cmd, "env\0", 4))
		return (ft_env());
	return (-1);
}
