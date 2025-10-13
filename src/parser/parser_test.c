/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 20:13:29 by serjimen         ###   ########.fr       */
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
