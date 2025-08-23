/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/21 18:48:03 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/// @brief Main function containing our Read Evaluate Print(Execute) Loop
/// @param envp data is saved as a linked list in 'environment'
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*environment;

	(void)argc;
	(void)argv;
	environment = NULL;
	if (!get_environment(envp, &environment))
		return (free_environment(&environment), perror("envp copy failed"), 1);
	while (1)
	{
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			command_in(input, &environment);
		}
		free(input);
	}
	free_environment(&environment);
	rl_clear_history();
	return (0);
}
