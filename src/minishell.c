/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/09 17:40:40 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/// @brief Main function containing our Read Evaluate Print(Execute) Loop
/// @param envp data is saved as a linked list in 'environment'
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*environment;
	int		exit_signal;
	//LEXER:
	t_token	*tokenlist;
	//PARSER:
	t_command	*commands;

	(void)argc;
	(void)argv;
	environment = NULL;
	tokenlist = NULL;
	commands = NULL;
	if (!get_environment(envp, &environment))
		return (free_environment(&environment), perror("envp copy failed"), 1);
	while (1)
	{
		exit_signal = 0;
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
			commands = parse_command(tokenlist);
			exit_signal = execute_all(commands, &environment);
			printf("exit status: %d\n", exit_signal);
			free_tokens(&tokenlist);
			free_commands(&commands);
		}
		free(input);
	}
	free_environment(&environment);
	rl_clear_history();
	return (0);
}
