/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:09:35 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/20 12:03:45 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_not_found(char *cmd, char **args)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	ft_free_split(args);
	exit (127) ;
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i ++;
	}
	free(split);
}

char	**ft_potential_paths(char **envp) //a partir de PATH=, guardar en paths las posibles direcciones accesibles deparadas por ":"
{
	char	**paths;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0) //encontrar línea con "PATH="
		i ++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':'); //separar los distintos paths delimitados por :
	return (paths);
}

char	*ft_build_full_path(char *command, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*path;
	int		i;

	full_path = NULL;
	paths = ft_potential_paths(envp); //posibles direcciones
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])//probar a juntar cada posible dirección con "/" + comando y ver si access(ejec) nos da el visto bueno
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, command);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (ft_free_split(paths), full_path);
		free(full_path);
		full_path = NULL;
		i ++;
	}
	return (ft_free_split(paths), NULL);
}

char	*ft_check_path(char *command, char **envp)
{
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(command, '/'))//si el comando contiene una dirección (por ej un programa nuestro), intentar ejecutarla
	{
		full_path = ft_strdup(command);// duplicamos porque más adelante haremos free sobre el string del comando
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else
		{
			free(full_path);
			return (NULL);
		}
	}
	else// si no, buscaremos dentro del sistema un programa que se corresponda( ej: cat, ls, grep...)
	{
		full_path = ft_build_full_path(command, envp);
		return (full_path);
	}
	return (NULL);
}
