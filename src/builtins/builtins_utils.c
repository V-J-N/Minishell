/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:34:38 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 10:19:48 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_utils.c
 * @brief Utility functions for identifying built-in commands and preparing
 * command arguments for execution.
 */
#include "minishell.h"

/**
 * @brief Checks if a given command name corresponds to any of the built-in
 * commands.
 * This function is used to determine if a command should be handled internally
 * by the shell instead of being searched for in the PATH (external command).
 * @param cmd The command name string.
 * @return bool True if the command is a built-in, false otherwise.
 */
bool	is_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) || \
	!ft_strncmp(cmd, "export", 7) \
	|| !ft_strncmp(cmd, "unset", 6) \
	|| !ft_strncmp(cmd, "exit", 5) \
	|| !ft_strncmp(cmd, "echo", 5) \
	|| !ft_strncmp(cmd, "pwd", 4) \
	|| !ft_strncmp(cmd, "env", 4));
}

/**
 * @brief Checks if a built-in command must be executed in the parent process.
 * Commands that modify the shell's state (environment variables or current
 * working directory) must run in the parent process. 'echo' and 'env' can run
 * in a child process (unless they are the only command).
 * @param cmd The command name string.
 * @return bool True if the command must run in the parent shell process.
 */
bool	is_parent_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) \
	|| !ft_strncmp(cmd, "export", 7) \
	||!ft_strncmp(cmd, "unset", 6) \
	|| !ft_strncmp(cmd, "pwd", 4) \
	|| !ft_strncmp(cmd, "exit", 5));
}

/**
 * @brief Counts the number of arguments in the linked list structure.
 * @param args The head of the `t_arg` list.
 * @return size_t The total count of arguments (including the command name).
 */
static size_t	args_count(t_arg *args)
{
	t_arg	*tmp;
	size_t	count;

	count = 0;
	tmp = args;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

/**
 * @brief Converts the linked list of arguments (`t_arg`) into a null-terminated
 * array of strings (`char **`).
 * This is essential because many built-ins (like `exit` or `cd`) and the
 * system call `execve` require arguments in the `char **` format.
 * @param args The head of the `t_arg` list.
 * @return char** A newly allocated, null-terminated array of duplicated argument
 * strings, or NULL on memory allocation failure. Must be freed
 * by `ft_free_array`.
 */
char	**args_to_array(t_arg *args)
{
	size_t	count;
	size_t	i;
	t_arg	*tmp;
	char	**arr;

	if (!args)
		return (NULL);
	count = args_count(args);
	arr = ft_calloc(count + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	tmp = args;
	i = 0;
	while (i < count)
	{
		arr[i] = ft_strdup(tmp->value);
		if (!arr[i])
			return (ft_free_array(arr), NULL);
		tmp = tmp->next;
		i++;
	}
	return (arr);
}
