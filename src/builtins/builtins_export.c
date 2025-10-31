/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:08:49 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/31 23:35:27 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file builtins_export.c
 * @brief Implementation of the 'export' built-in command, handling environment
 * variable assignment, printing, and identifier validation.
 */
#include "minishell.h"

/**
 * @brief Performs the actual assignment or update of an environment
 * variable node.
 * This is the low-level function that modifies the `t_env` list. It handles
 * two scenarios:
 * 1 - Update: If the key exists, it frees the old value/full string and
 * sets the new ones.
 * 2 - Creation: If the key doesn't exist, it creates a new node and
 * adds it to the end.
 * @param key The variable key.
 * @param new_v The new value.
 * @param env A pointer to the head of the environment list.
 * @param full_var The complete string "KEY=VALUE".
 */
static void	ft_assignation(char *key, char *new_v, t_env **env, char *full_var)
{
	t_env	*node;
	t_env	*new_node;

	node = find_node_by_key(*env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(new_v);
		if (!node->value)
			return ;
		free(node->full_env);
		node->full_env = ft_strdup(full_var);
		if (!node->full_env)
			return ;
	}
	else
	{
		new_node = ft_lstnew_mini_env(key, new_v, full_var);
		if (!new_node)
			return ;
		ft_addback_mini_env(env, new_node);
	}
	return ;
}

/**
 * @brief Splits a string of the form "KEY=VALUE" and assigns it
 * to the environment.
 * This function serves as a wrapper to separate the key and value before
 * calling `ft_assignation`. It's also used by `ft_cd` for PWD/OLDPWD updates.
 * @param full_var The string containing the assignment.
 * @param env A pointer to the head of the environment list.
 * @return int 0 for success, 1 for failure (memory allocation).
 */
int	ft_assign_in(char *full_var, t_env **env)
{
	char	*key;
	char	*value;

	key = set_key(full_var);
	if (!key)
		return (1);
	value = set_value(full_var);
	if (!value)
	{
		free(key);
		return (1);
	}
	ft_assignation(key, value, env, full_var);
	free(key);
	free(value);
	return (0);
}

/**
 * @brief Creates a new environment node for a variable without a
 * value assignment.
 * When the format is `export VAR` (no '='), the variable is added to the
 * environment but has a NULL value (it's "set but null"). It still appears
 * in `export -p`.
 * @param key The variable key string.
 * @param env A pointer to the head of the environment list.
 */
static void	ft_no_assignation(char *key, t_env **env)
{
	t_env	*new;
	t_env	*node;

	node = find_node_by_key(*env, key);
	if (!node)
	{
		new = ft_lstnew_mini_env(key, NULL, key);
		if (new)
			ft_addback_mini_env(env, new);
	}
	return ;
}

/**
 * @brief Iterates through all arguments and processes them for export.
 * This function handles validation and dispatches the argument to either
 * `ft_assign_in` (if '=' is present) or `ft_no_assignation`
 * (if no '=' is present).
 * @param args The null-terminated array of arguments (starting from args[1]).
 * @param env A pointer to the head of the environment list.
 * @return int 0 for success, 1 if any memory allocation or identifier
 * error occurred.
 */
static int	navigate_args(char **args, t_env **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_not_valid(args[i]);
			i ++;
			continue ;
		}
		if (ft_strchr(args[i], '='))
		{
			if (ft_assign_in(args[i], env) == 1)
				return (1);
		}
		else
			ft_no_assignation(args[i], env);
		i++;
	}
	return (0);
}

/**
 * @brief Implements the 'export' built-in command.
 * If arguments are provided, it attempts to create or redefine
 * environment variables.
 * If no arguments are provided (`export` alone), it prints the sorted
 * list of environment variables in the 'export' format
 * (`declare -x KEY="VALUE"`).
 * @param cmd A pointer to the 't_command' node containing the arguments.
 * @param env A double pointer to the environment list head (t_env**).
 * @return int The exit status: 0 for success, 1 on allocation failure or
 * invalid identifier.
 */
int	ft_export(t_command *cmd, t_env **env)
{
	char	**args_arr;
	int		exit_status;

	if (!cmd || !env)
		return (EXIT_FAILURE);
	args_arr = args_to_array(cmd->args);
	if (!args_arr)
		return (EXIT_FAILURE);
	if (!args_arr[1])
	{
		print_sorted_env(*env);
		ft_free_array(args_arr);
		return (EXIT_SUCCESS);
	}
	exit_status = navigate_args(args_arr, env);
	ft_free_array(args_arr);
	return (exit_status);
}
