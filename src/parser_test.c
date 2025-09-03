/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/03 14:44:38 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Testeo del parser
 * 
 */
void	print_commands(t_command *commands)
{
	t_command	*temp;
	t_redir		*redir;
	int			i;

	temp = commands;
	i = 0;
	while (temp)
	{
		while (temp->cmd_args[i])
		{
			if (i == 0)
				printf("Command: [%s]\n", temp->cmd_args[i]);
			else
				printf("Arguments or Flags: [%s]\n", temp->cmd_args[i]);
			i++;
		}
		printf("Number of Commands: %d\n", (int)temp->cmd_argc);
		printf("Type of Command: 1 - CMD_SIMPLE = %d\n", temp->type);
		redir = temp->redirs;
		while (redir)
		{
			printf("TYPE %d REDIR OUT\n", redir->type);
			printf("Target file outfile: [%s]", redir->outfile);
			redir = redir->next;
		}
		temp = temp->next;
	}
}

int	main(int argc, char *argv[])
{
	t_token		*list;
	t_command	*commands;

	list = NULL;
	commands = NULL;
	(void)argc;
	if (argv[1])
	{
		list = tokenizer(argv[1]);
		commands = parse_command(list);
	}
	print_list(list);
	print_commands(commands);
	free_tokens(&list);
	free_commands(&commands);
	return (0);
}
