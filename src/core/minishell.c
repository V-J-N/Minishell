/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/17 13:43:12 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_data	*init_data(char **envp)
{
	t_data			*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = NULL;
	data->token = NULL;
	data->parsed = NULL;
	if (!get_environment(envp, &data->env))
	{
		perror("envp copy failed");
		free(data);
		return (NULL);
	}
	return (data);
}

static int	sigint_check(int exit_signal)
{
	if (g_sigint_status == SIGINT)
	{
		g_sigint_status = 0;
		return (130);
	}
	else if (g_sigint_status == SIGQUIT)
	{
		g_sigint_status = 0;
		return (131);
	}
	return (exit_signal);
}

static int	execute(t_data *data, int exit_signal, char *input)
{
	exit_signal = execute_all(data);
	printf("exit status: %d\n", exit_signal); //borrar al final!
	ft_cleanup_loop(data, input, 0);
	return (exit_signal);
}

static int	rep_loop(t_data *data, int exit_signal, char *input)
{
	while (1)
	{
		exit_signal = sigint_check(exit_signal);
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			data->token = tokenizer(input);
			if (data->token)
				data->parsed = parse_command(data->token);
			if (!data->token || !data->parsed)
			{
				ft_cleanup_loop(data, input, 1);
				continue ;
			}
			exit_signal = execute(data, exit_signal, input);
		}
	}
	return (exit_signal);
}

/// @brief Main function containing our Read Evaluate Print(Execute) Loop
/// @param envp data is saved as a linked list in 'environment'
int	main(int argc, char **argv, char **envp)
{
	char			*input;
	int				exit_signal;
	t_data			*data;

	(void)argc;
	(void)argv;
	input = NULL;
	data = init_data(envp);
	if (!data)
		return (1);
	setup_signals();
	exit_signal = 0;
	exit_signal = rep_loop(data, exit_signal, input);
	ft_cleanup_end(data);
	return (exit_signal);
}
