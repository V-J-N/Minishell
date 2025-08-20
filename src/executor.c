/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:12:05 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/20 12:04:32 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd_str, t_env **envlist)
{
	char	**args;
	char	*cmd;
	char	**envarr;

	args = ft_split(cmd_str, ' ');//separar argumentos: por ej "ls -l"
	if (!args || !args[0])
		return ;
	envarr = envlist_to_arr(envlist);
	cmd = ft_check_path(args[0], envarr);//conseguir path válido
	if (!cmd)
		cmd_not_found(args[0], args);
	execve(cmd, args, envarr); // El proceso actual es reemplazado completamente por un nuevo proceso,
	// No hay fugas de memoria porque ese proceso ya no existe como tal
	perror("execve"); // pero cerramos limpiamente porque si execve falla, sigue ejecutando estas líneas de código
	ft_free_split(args);
	free(cmd);
	exit (1);
}

void	command_in(char *command, t_env **environment)
{
	pid_t		pid;

	pid = fork();//iniciar proceso hijo
	if (pid < 0)
		return (perror("fork"));
	if (pid == 0)//proceso hijo:
		execute_command(command, environment);
	waitpid(pid, NULL, 0);//esperar en proceso padre a que termine el hijo
	return ;
}
