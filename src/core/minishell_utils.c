/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:23:54 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/17 13:31:31 by vjan-nie         ###   ########.fr       */
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
	g_sigint_status = signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return ;
}

void	ft_cleanup_loop(t_data *data, char *input, bool	error)
{
	if (data)
	{
		if (data->token)
			free_tokens(&data->token);
		if (data->parsed)
			free_parser(&data->parsed);
	}
	if (input)
		free(input);
	if (error)
		printf("Syntax Error\n");
	return ;
}

void	ft_cleanup_end(t_data *data)
{
	if (data)
	{
		if (data->token)
			free_tokens(&data->token);
		if (data->parsed)
			free_parser(&data->parsed);
		if (data->env)
			free_environment(&data->env);
		free(data);
	}
	rl_clear_history();
	return ;
}
