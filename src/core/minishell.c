/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/11 13:16:26 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Main function containing our Read Evaluate Print(Execute) Loop
/// @param envp data is saved as a linked list in 'environment'
int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env			*environment;
	int				exit_signal;
	//LEXER:
	t_token			*tokenlist;
	//PARSER:
	t_parse_state	*parse_state;
	
	(void)argc;
	(void)argv;
	environment = NULL;
	tokenlist = NULL;
	parse_state = NULL;
	if (!get_environment(envp, &environment))
		return (free_environment(&environment), perror("envp copy failed"), 1);
	setup_signals();
	exit_signal = 0;
	while (1)
	{
		if (g_last_signal == SIGINT)
		{
			exit_signal = 130;
			g_last_signal = 0;
		}
		else if (g_last_signal == SIGQUIT)
		{
			exit_signal = 131;
			g_last_signal = 0;
		}
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			tokenlist = tokenizer(input);
			// print_list(tokenlist);
			if (!tokenlist)
			{
				printf("Syntax Error\n");
				continue ;
			}
			parse_state = parse_command(tokenlist);
			// print_commands(parse_state);
			if (!parse_state)
			{
				printf("Syntax Error\n"); // Hay leaks que tb provienen del lexer
				continue ;
			}
			exit_signal = execute_all(parse_state->cmd_list, &environment);
			printf("exit status: %d\n", exit_signal);
			free_tokens(&tokenlist);
			free_parser(&parse_state);
		}
		free(input);
	}
	free_environment(&environment);
	rl_clear_history();
	return (exit_signal);
}
