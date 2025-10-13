/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:22:23 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/13 18:14:43 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * 
 */
#include "minishell.h"

/**
 * 
 */
void	trim_quotes(t_lexer *tokenlist, t_command *commandlist)
{
	t_token		*temp;
	t_command	*cmd;
	size_t		i;

	temp = tokenlist;
	cmd = commandlist;
	while (temp)
	{
		i = 0;
		if (temp->has_quotes == true && temp->quote == SINGLE)
		{
			while (cmd->args->value)
			{
				
				cmd->args->value = cmd->args->next;
			}
		}
		temp = temp->next;
	}
}

/**
 * 
 */
void	expander(t_lexer *tokenlist, t_command *commandlist)
{
	t_token	*temp;

	temp = tokenlist;
	while (temp)
	{
		if (temp->has_quotes)
		{
			trim_quotes(tokenlist, commandlist);
		}
		temp = temp->next;
	}
}