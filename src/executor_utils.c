/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:09:35 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/21 18:47:12 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Manage invalid command sent, finish child process
void	cmd_not_found(char *cmd, char **args)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	ft_free_array(args);
	exit (127) ;
}

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i ++;
	}
	free(array);
}

/// @brief Separate potential paths from env by spliting with the delimitator ':'
char	**ft_potential_paths(char *path_value)
{
	char	**paths;

	if (!path_value)
		return (NULL);
	paths = ft_split(path_value + 5, ':');
	return (paths);
}

/// @brief Check potential paths by building the strings (paht + '/' + command)
char	*ft_build_full_path(char *command, char *path_value)
{
	char	**paths;
	char	*full_path;
	char	*path;
	int		i;

	full_path = NULL;
	paths = ft_potential_paths(path_value);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, command);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (ft_free_array(paths), full_path);
		free(full_path);
		full_path = NULL;
		i ++;
	}
	return (ft_free_array(paths), NULL);
}

/// @brief Get full path from a command string, and check if it can be executed
/// @warning !!! Estás bien al asumir que el usuario pasó una ruta (ej. ./myprog), 
/// pero recuerda: Si es un path relativo, puede necesitar getcwd() para ejecutarse correctamente.
/// Si estás en un entorno tipo minishell, deberías manejar también el caso ~/.
char	*ft_check_path(char *command, char *path_value)
{
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(command, '/'))
	{
		full_path = ft_strdup(command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else
		{
			free(full_path);
			return (NULL);
		}
	}
	else
	{
		full_path = ft_build_full_path(command, path_value);
		return (full_path);
	}
	return (NULL);
}
