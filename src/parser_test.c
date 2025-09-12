/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/11 13:14:15 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Testeo del parser
 * 
 */
void	print_commands(t_parse_state *commands)
{
	t_parse_state	*temp;
	t_redir			*redir;
	t_arg			*temp_args;

	temp = commands;
	while (temp)
	{
		temp_args = temp->cmd_node->args;
		while (temp_args)
		{
			printf("Arguments: [%s]\n", temp_args->value);
			temp_args = temp_args->next;
		}
		printf("Number of Commands: %d\n", (int)temp->cmd_node->cmd_argc);
		redir = temp->cmd_node->redirs;
		while (redir)
		{
			printf("TYPE %d REDIR\n", redir->type);
			printf("Target file: [%s]\n", redir->file);
			redir = redir->next;
		}
		break ;
		// Bucle infinito no es una lista.
	}
}

/* int	main(int argc, char *argv[])
{
	t_token			*list;
	t_parse_state	*commands;

	list = NULL;
	commands = NULL;
	(void)argc;
	if (argv[1])
	{
		list = tokenizer(argv[1]);
		if (!list)
			return (-1);
		commands = parse_command(list);
		if (!commands)
			return (-1);
	}
	print_list(list);
	print_commands(commands);
	free_tokens(&list);
	free_parser(&commands);
	return (0);
} */
