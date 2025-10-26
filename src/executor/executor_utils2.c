/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:25:00 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/26 09:31:23 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief Extracts the command and flags saved
* in the parsed command structure, and returns
* an array of strings to be used by execute_command()
 */
char	**command_to_arr(t_command *command)
{
	char	**command_array;
	t_arg	*temp;
	size_t	i;

	if (!command)
		return (NULL);
	command_array = NULL;
	command_array = malloc(sizeof(char *) * (command->cmd_argc + 1));
	if (!command_array)
		return (NULL);
	temp = command->args;
	i = 0;
	while (temp)
	{
		command_array[i] = ft_strdup(temp->value);
		if (!command_array[i])
		{
			ft_free_array(command_array);
			return (NULL);
		}
		temp = temp->next;
		i ++;
	}
	command_array[i] = NULL;
	return (command_array);
}

/**
* @brief The raw value in status given by wait()
* is processed by the corresponding <sys/wait.h> macros,
* in order to get the error code number.
 */
int	ft_wait_and_exit(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		exit_code;

	exit_code = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	return (exit_code);
}
