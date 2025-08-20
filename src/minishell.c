/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/20 12:14:51 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*environment;
	// char	**envp_copy;

	(void)argc;
	(void)argv; // no queremos usarlas aunque existan: que el compilador las ignore(si no -Wall, etc. salta): sabemos que las ignoramos, vamos
	// envp_copy = copy_env(envp); //copiar variables de entorno para poder modificarlas
	environment = NULL;
	if (!get_environment(envp, &environment))
		return (free_environment(&environment), perror("envp copy failed"), 1);
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
			command_in(input, &environment); //ejecutar comando
		}
		free(input);
	}
	// free_envp_copy(envp_copy);//liberar copia de envp
	free_environment(&environment);
	rl_clear_history();// eliminar history entre sesiones
	return (0);
}
