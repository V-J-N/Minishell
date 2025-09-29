/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:23:54 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/29 05:45:05 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	sigint_handler(int signum)
{
	g_last_signal = signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();//avisa a readline que empezamos nueva línea
	rl_replace_line("", 0);//borra el contenido actual de la línea comenzada
	rl_redisplay();//vuelve a mostrar prompt limpio
	return ;
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler); // Ctrl-C (no llamo a la fc, sino que asigno su dirección)
	signal(SIGQUIT, SIG_IGN);// Ctrl-\ (captura pero ignoramos)
	return ;
}
