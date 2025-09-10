/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:25:00 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/10 11:48:39 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_envlst_size(t_env *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}

/**
 * @brief Extracts the data from our
 * environment data structure list and returns
 * an array of strings that can be used by execve()
 * */
char	**envlist_to_arr(t_env **envlist)
{
	int		count;
	char	**envarr;
	t_env	*temp;
	int		i;

	count = ft_envlst_size(*envlist);
	envarr = ft_calloc(count + 1, sizeof(char *));
	if (!envarr)
		return (NULL);
	temp = *envlist;
	i = 0;
	while (temp)
	{
		envarr[i] = ft_strdup(temp->full_env);
		if (!envarr[i])
			return (ft_free_array(envarr), NULL);
		temp = temp->next;
		i++;
	}
	envarr[i] = NULL;
	return (envarr);
}

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
	int	status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0) //esperamos a todos los hijos
	{
		if (pid == last_pid) // pero sólo recuperamos el id del último
		{// Ahora usamos macros para extraer el código de salida de un proceso hijo:
			if (WIFEXITED(status))//el hijo ha salido con exit()?
				return (WEXITSTATUS(status));//el valor que se paso a exit(x)
			else if (WIFSIGNALED(status))//o se ha terminado con señal? (ej. Ctrl+C)
				return (128 + WTERMSIG(status));//qué señal se ejecutó (ej. SIGINT = 2)
		}
	}
	return (EXIT_FAILURE); // Si no conseguimos capturar el último hijo por alguna razón
}
