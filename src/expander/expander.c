/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:24:54 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/23 10:43:44 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 *
 */
#include "minishell.h"

/**
 *
 */
static t_token_quote	check_has_quotes(char *str)
{
	size_t			i;
	t_token_quote	quote;

	i = 0;
	quote = NONE;
	while (str[i])
	{
		if (str[i] == 39)
			return (quote = SINGLE);
		else if (str[i] == 34)
			return (quote = DOUBLE);
		i++;
	}
	return (quote);
}

/**
 *
 */
static t_arg	*check_arguments(t_arg *args)
{
	t_arg	*temp;

	temp = args;
	while (temp)
	{
		if (temp->quote == NONE && temp->has_quotes == true)
			temp->quote = check_has_quotes(temp->value);
		if (temp->has_quotes)
		{
			if (temp->quote == SINGLE)
				check_singles_arg(temp);
			else if (temp->quote == DOUBLE)
				check_doubles_arg(temp);
		}
		if (temp->exp_value != NULL)
			change_value(temp);
		temp = temp->next;
	}
	return (temp);
}

/**
 *
 */
static t_redir	*check_redirs(t_redir *redirs)
{
	t_redir	*temp;

	temp = redirs;
	while (temp)
	{
		if (temp->quote == NONE && temp->has_quotes == true)
			temp->quote = check_has_quotes(temp->file);
		if (temp->has_quotes)
		{
			if (temp->quote == SINGLE)
				check_singles_redir(temp);
			if (temp->quote == DOUBLE)
				check_doubles_redir(temp);
		}
		if (temp->exp_file != NULL)
			change_file(temp);
		temp = temp->next;
	}
	return (temp);
}

/**
 *
 */
t_command	*expander(t_command *cmd_list, t_env *env, int exit_signal)
{
	t_command	*tmp;
	t_arg		*temp;
	t_redir		*temp2;

	(void)env;
	tmp = cmd_list;
	while (tmp)
	{
		temp = tmp->args;
		expand_args(temp, env, exit_signal);
		temp2 = tmp->redirs;
		expand_redirs(temp2, env, exit_signal);
		temp = tmp->args;
		check_arguments(temp);
		temp2 = tmp->redirs;
		check_redirs(temp2);
		tmp = tmp->next;
	}
	return (cmd_list);
}
