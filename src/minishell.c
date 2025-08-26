/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:11:50 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/26 17:08:26 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/// @brief Main function containing our Read Evaluate Print(Execute) Loop
/// @param envp data is saved as a linked list in 'environment'
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*environment;
	//int		in;
	//int		out;
	char	**pipe_args;

	(void)argc;
	(void)argv;
	environment = NULL;
	if (!get_environment(envp, &environment))
		return (free_environment(&environment), perror("envp copy failed"), 1);
	while (1)
	{
		//in = -1;
		//out = -1;
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			pipe_args = ft_split((const char*)input, '|');
			//lexer:
			//if input_file, output_file, here_doc and/or append:
				//in = get_inputfile_fd;
				//if (in == -1)
					//error;
				//in = get_heredoc_fd();
				//if (in == -1)
					//error;
				//out = get_outputfile_fd();
				//if (out == -1)
					//error;
				//out = get_append_fd();
				//if (out == -1)
					//error;
			//if command:
			//command_in(input, &environment);
			//if pipe:
				//pipes();
			pipes(pipe_args, 3, &environment, 0, 1);// escribo manualmente como si hubiesen sido separados: ls -l | wc -l | cat
			ft_free_array(pipe_args);
		}
		free(input);
		//close(in);
		//close(out);
	}
	free_environment(&environment);
	rl_clear_history();
	return (0);
}
