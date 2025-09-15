/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/15 11:10:45 by sergio-jime      ###   ########.fr       */
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
	size_t			i;

	temp = commands;
	list = temp->cmd_list;
	i = 0;
	while (list)
	{
		printf("==== Node List [%ld]\n", i);
		printf("-> Command [%s]\n", list->args->value);
		list->args = list->args->next;
		if (list->cmd_argc > 0)
			while (list->args)
			{
				printf("----> Arguments [%s]\n", list->args->value);
				list->args = list->args->next;
			}
		if (list->redirs)
			while (list->redirs)
			{
				printf("|__> Redirs [%s]\n", list->redirs->file);
				list->redirs = list->redirs->next;
			}
		list = list->next;
		i++;
	}
}

int	main(int argc, char *argv[])
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
}
