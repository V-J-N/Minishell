/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:55:01 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/14 11:24:43 by serjimen         ###   ########.fr       */
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
	// t_command		*list;
	// t_redir			*redir;
	size_t			i;

	temp = commands;
	i = 0;
	while (temp->cmd_list)
	{
		printf("==== Node List [%ld]\n", i);
		printf("Value: [%s]\n", temp->cmd_list->args->value);
		printf("QUOTE: [%d]\n", temp->cmd_list->quote);
		printf("is_expanded: [%d]\n", temp->cmd_list->is_expanded);
		printf("has_quotes: [%d]\n", temp->cmd_list->has_quotes);
/* 		if (list->args)
		{
			printf("-> Command [%s]\n", list->args->value);
			list->args = list->args->next;
		}
		if (list->cmd_argc > 0)
		{
			while (list->args)
			{
				printf("----> Arguments [%s]\n", list->args->value);
				list->args = list->args->next;
			}
		}
		redir = list->redirs;
		while (redir)
		{
			printf("|__> Redirs [%s]\n", redir->file);
			redir = redir->next;
		} */
		temp->cmd_list = temp->cmd_list->next;
		i++;
	}
}
