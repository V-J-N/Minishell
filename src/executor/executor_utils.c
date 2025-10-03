/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:09:35 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/03 11:55:56 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Manage invalid command, clean, finish child process
 */
void	cmd_not_found(char *cmd, char **env_arr, char **args)
{
	if (ft_strchr(cmd, '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ft_free_array(args);
		ft_free_array(env_arr);
	}
	else
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("\n", 2);
		ft_free_array(args);
		ft_free_array(env_arr);
	}
	exit(127);
}

int	is_directory(const char *path)
{
	struct stat	statbuf;
	//Esta estructura contiene información sobre 
	//un archivo o directorio, como permisos, tamaño, tipo, etc.

	if (stat(path, &statbuf) != 0) //obtiene información del archivo o directorio apuntado por path y la guarda en statbuf
		return (0);
	return (S_ISDIR(statbuf.st_mode));
	//statbuf.st_mode contiene los permisos y tipo de archivo.
	//S_ISDIR(...) es un macro que devuelve true (distinto de 0) 
	//si st_mode indica que es un directorio.
}

/**
* @brief Separate potential paths from env by spliting ':'
*/
char	**ft_potential_paths(char **envp)
{
	char	**paths;
	int		i;

	i = 0;
	if (!envp[i] || ft_strlen(envp[i]) < 6)
		return (NULL);
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i ++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

/** 
 * @brief Check potential paths by building the strings 
 * (path + '/' + command) 
 */
char	*ft_build_full_path(char *command, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*path;
	int		i;

	paths = ft_potential_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		if (!path)
			break ;
		full_path = ft_strjoin(path, command);
		free(path);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
			return (ft_free_array(paths), full_path);
		free(full_path);
		full_path = NULL;
		i ++;
	}
	return (ft_free_array(paths), NULL);
}

/**
* @brief Get full path from a command string, and check if it can be executed
* @warning !!! Estás bien al asumir que el usuario pasó una ruta (ej. ./myprog), 
* si es un path relativo, puede necesitar getcwd() para ejecutarse correctamente.
* Si estás en un entorno tipo minishell, deberías manejar también el caso ~/.
*/
char	*ft_check_path(char *command, char **envp)
{
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(command, '/'))
	{
		full_path = ft_strdup(command);
		if (access(full_path, X_OK) == 0 && !is_directory(full_path))
			return (full_path);
		else
		{
			free(full_path);
			return (NULL);
		}
	}
	else
	{
		full_path = ft_build_full_path(command, envp);
		return (full_path);
	}
	return (NULL);
}
