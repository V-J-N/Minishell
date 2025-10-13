/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:49:50 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 17:59:28 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file environment.c
 * @brief Functions for environment variable initialization and management.
 */
#include "minishell.h"

/**
 * @brief Performs a safety check on the dynamically allocated key and value
 * strings.
 * This utility function is designed to check for memory allocation failures
 * that may have occurred during the extraction of the key and value of an
 * environment variable.
 * @param key The dynamically allocated key string.
 * @param value The dynamically allocated value string.
 * @return true If both key and value are valid.
 * @return false If either key or value is NULL, indicating an error.
 */
static bool	verify_env(char *key, char *value)
{
	if (!key || !value)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		return (false);
	}
	return (true);
}

/**
 * @brief Initializes the internal environment linked list from the 'envp' array
 * This function iterates through the NULL-terminated 'envp' array provided
 * by the parent process. to construct the shell's internal 't_env' linked list.
 * This internal list provides a mutable and structured representation of the
 * environment.
 * @param envp A NULL-terminated array of strings, where each string is an
 * environment variable in the format "KEY=value". This is the source of the
 * environment data.
 * @param environment A pointer to a pointer to the head of the 't_env' linked
 * list. This double pointer is used to modify the head of the list directly,
 * allowing the function to build the list from scrath.
 * @return true On successful initialization of all valid variables.
 * @return false If a memory allocation failure occurs at any point during
 * key/value extraction or node creation.
 */
bool	get_environment(char *envp[], t_env **environment)
{
	t_env	*new_node;
	size_t	i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != NULL)
		{
			key = set_key(envp[i]);
			value = set_value(envp[i]);
			if (!verify_env(key, value))
				return (false);
			new_node = ft_lstnew_mini_env(key, value, envp[i]);
			if (!new_node)
				return (free(key), free(value), false);
			ft_addback_mini_env(environment, new_node);
			free(key);
			free(value);
		}
		i++;
	}
	return (true);
}
