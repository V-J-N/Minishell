/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:10:27 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/01 15:11:02 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser.c
 * @brief 
 */
#include "minishell.h"

static bool	fill_block(t_command *block, t_token *tokens)
{
	t_token		*temp;
	size_t		i;

	temp = tokens;
	i = 0;
	while(temp && temp->type == WORD)
	{
		block->cmd_args[i] = ft_strdup(temp->value);
		if (!block->cmd_args[i])
			return (ft_free_array(block->cmd_args), false);
		temp = temp->next;
		i++;
	}
	block->type = CMD_SIMPLE;
	return (true);
}

static t_command	*init_block(t_token *tokens)
{
	t_token		*temp;
	t_command	*block;

	block = NULL;
	temp = tokens;
	block = ft_calloc(1, sizeof(t_command));
	if (!block)
		return (NULL);
	block->next = NULL;
	block->type = CMD_NONE;
	block->cmd_argc = lstsize_token_word(temp);
	block->cmd_args = ft_calloc(block->cmd_argc + 1, sizeof(char *));
	if (!block->cmd_args)
		return (free(block), NULL);
	return (block);
}

t_command	*parse_command(t_token *tokens)
{
	t_command	*cmd_block;

	cmd_block = NULL;
	if (!tokens)
		return (NULL);
	if (tokens->type != WORD)
		return (printf("Necesitas un comando! de primeras"), NULL);
	cmd_block = init_block(tokens);
	if (!fill_block(cmd_block, tokens))
		return (free(cmd_block), NULL);
	return (cmd_block);
}
