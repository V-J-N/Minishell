/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/19 14:50:30 by sergio-jime      ###   ########.fr       */
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
	t_command		*list;
	t_arg			*arg;
	t_redir			*redir;
	size_t			i;

	temp = commands;
	list = temp->cmd_list;
	i = 0;
	while (list)
	{
		printf("==== Node List [%ld]\n", i);
		if (list->args)
		{
			printf("-> Command [%s]\n", list->args->value);
			list->args = list->args->next;
		}
		if (list->cmd_argc > 0)
		{
			while (arg)
			{
				printf("----> Arguments [%s]\n", arg->value);
				arg = arg->next;
			}
		}
		redir = list->redirs;
		while (redir)
		{
			printf("|__> Redirs [%s]\n", redir->file);
			redir = redir->next;
		}
		list = list->next;
		i++;
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
	//print_list(list);
	print_commands(commands);
	free_tokens(&list);
	free_parser(&commands);
	return (0);
} */
