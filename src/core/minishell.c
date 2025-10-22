/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/22 14:09:17 by vjan-nie         ###   ########.fr       */
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

static int	execute(t_data *data, int exit_signal)
{
	exit_signal = execute_all(data);
	printf("exit status: %d\n", exit_signal); //borrar al final!
	return (exit_signal);
}

static int	rep_loop(t_data *data, int exit_signal, char *input, int inter)
{
	while (1)
	{
		exit_signal = sigint_check(exit_signal);
		if (inter)
			input = readline("$> ");
		else
			input = get_next_line(STDIN_FILENO);
		if (!input)
		{
			if (inter)
				printf("exit\n");
			break ;
		}
		if (*input)
		{
			if (inter)
				add_history(input);
			data->token = tokenizer(input);
			if (data->token)
				data->parsed = parse_command(&data->token);
			if (!data->token || !data->parsed)
			{
				ft_cleanup_loop(data, input, 1);
				continue ;
			}
			data->parsed->cmd_list = expander(data->parsed->cmd_list, data->env, exit_signal);
			exit_signal = execute(data, exit_signal);
			ft_cleanup_loop(data, input, 0);
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
	int				interactive;

	(void)argc;
	(void)argv;
	input = NULL;
	data = init_data(envp);
	if (!data)
		return (1);
	if (!shell_lvl_handler(data))
		return (1);
	setup_signals();
	exit_signal = 0;
	interactive = isatty(STDIN_FILENO);
	exit_signal = rep_loop(data, exit_signal, input, interactive);
	ft_cleanup_end(data);
	return (exit_signal);
}
