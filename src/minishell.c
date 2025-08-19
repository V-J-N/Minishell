/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/17 17:45:13 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**envp_copy;

	(void)argc;
	(void)argv; // no queremos usarlas aunque existan: que el compilador las ignore(si no -Wall, etc. salta): sabemos que las ignoramos, vamos
	envp_copy = copy_env(envp); //copiar variables de entorno para poder modificarlas
	if (!envp_copy)
		return (perror("envp copy failed"), 1);
	while (1) // REPLoop
	{
		input = readline("$> "); // readline (se puede usar log?): muestra prompt y lee lo que introducimos
		if (!input) // EOF (Ctrl+D)
		{
			printf("exit\n");
			break ;
		}
		if (*input) // Si la línea no está vacía añadir al historial de comandos
		{
			add_history(input);
			command_in(input, envp_copy); //ejecutar comando
		}
		free(input);
	}
	free_envp_copy(envp_copy);//liberar copia de envp
	rl_clear_history();// eliminar history entre sesiones
	return (0);
}
