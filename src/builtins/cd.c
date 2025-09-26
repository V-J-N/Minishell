/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:34:58 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 13:58:39 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_export_str(t_env **env, char *full_env)
{
	t_env	*node;
	char	*key;
	char	*value;
	char	*full_copy;
	t_env	*new_node;

	key = set_key(full_env);
	if (!key)
		return (1);

	node = find_node_by_key(*env, key);
	if (node)
	{
		// Si ya existe, liberar cadenas viejas
		free(node->key);
		free(node->value);
		free(node->full_env);

		value = set_value(full_env);
		full_copy = ft_strdup(full_env);

		if (!value || !full_copy)
		{
			free(key);
			free(value);
			free(full_copy);
			return (1);
		}

		node->key = key;
		node->value = value;
		node->full_env = full_copy;
	}
	else
	{
		value = set_value(full_env);
		full_copy = ft_strdup(full_env);

		if (!value || !full_copy)
		{
			free(key);
			free(value);
			free(full_copy);
			return (1);
		}

		new_node = ft_lstnew_mini_env(key, value, full_copy);
		if (!new_node)
		{
			free(key);
			free(value);
			free(full_copy);
			return (1);
		}

		// Las copias ya están duplicadas dentro del nodo, liberar las originales
		free(key);
		free(value);
		free(full_copy);

		ft_addback_mini_env(env, new_node);
	}
	return (0);
}

int	ft_cd(t_command *cmd, t_env **env)
{
	char	cwd[4096];   // PATH_MAX o 4096
	char	*oldpwd;
	t_env	*pwd_node;
	t_env	*oldpwd_node;
	char	**args;
	char	*path;

	args = args_to_array(cmd->args);
	if (!args)
		return (1);

	// Si no hay argumento, usa HOME
	if (!args[1])
	{
		t_env *home_node = find_node_by_key(*env, "HOME");
		if (!home_node || !home_node->value)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			ft_free_array(args);
			return (1);
		}
		path = home_node->value;
	}
	else
		path = args[1];

	// Obtener directorio actual para OLDPWD
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd");
		ft_free_array(args);
		return (1);
	}

	// Cambiar directorio
	if (chdir(path) == -1)
	{
		perror("cd");
		ft_free_array(args);
		return (1);
	}

	// Guardar OLDPWD = cwd actual antes de cambiar
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
	{
		ft_free_array(args);
		return (1);
	}

	// Actualizar OLDPWD
	oldpwd_node = find_node_by_key(*env, "OLDPWD");
	if (oldpwd_node)
	{
		free(oldpwd_node->value);
		oldpwd_node->value = oldpwd;
		free(oldpwd_node->full_env);
		oldpwd_node->full_env = ft_strjoin("OLDPWD=", oldpwd);
	}
	else
	{
		char *full_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
		ft_export_str(env, full_oldpwd);
		free(full_oldpwd);
		free(oldpwd);
	}

	// Obtener nuevo cwd para actualizar PWD
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd");
		ft_free_array(args);
		return (1);
	}

	// Actualizar PWD
	pwd_node = find_node_by_key(*env, "PWD");
	if (pwd_node)
	{
		free(pwd_node->value);
		pwd_node->value = ft_strdup(cwd);
		free(pwd_node->full_env);
		pwd_node->full_env = ft_strjoin("PWD=", cwd);
	}
	else
	{
		char *full_pwd = ft_strjoin("PWD=", cwd);
		ft_export_str(env, full_pwd);
		free(full_pwd);
	}

	ft_free_array(args);
	return (0);
}
