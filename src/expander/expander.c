/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:24:54 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/20 15:11:54 by serjimen         ###   ########.fr       */
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
t_command *expander(t_command *cmd_list, t_env *env)
{
	t_command	*tmp;
	t_arg		*temp;
	t_redir		*temp2;
	char		*env_key;
	char		*variable;
	size_t		i;
	// size_t		j;
	size_t		k;
	size_t		len;

	env_key = NULL;
	variable = NULL;
	tmp = cmd_list;
	while (tmp)
	{
		temp = tmp->args;
		while (temp)
		{
			if (temp->is_expanded && contains_dollar(temp->value))
			{
				i = 0;
				// j = 0;
				k = 0;
				len = ft_strlen(temp->value);
				temp->exp_value = ft_calloc(len + 1, sizeof(char));
				if (!temp->exp_value)
					return (NULL);
				env_key = ft_calloc(len + 1, sizeof(char));
				if (!env_key)
					return (NULL);
				while (temp->value)
				{
					if (temp->value[i] == 36)
					{
						i++;
						if (temp->value[i] == 63)
						{
							temp->exp_value = ft_itoa(g_sigint_status);
							temp->exp_value = '\0';
							i++;
							change_value(temp);
							continue;
						}
						else if (ft_isdigit(temp->value[i]))
						{
							i++;
							continue;
						}
						while (ft_isalnum(temp->value[i]) || temp->value[i] == '_')
						{
							env_key[k] = temp->value[i];
							i++;
							k++;
						}
						env_key[k] = '\0';
						variable = get_value_by_key(env, env_key);
						temp->exp_value = variable;
						// free(variable);
						change_value(temp);
					}
					break ;
					// Seguir buscando chars
				}
			}
			temp = temp->next;
		}
		check_arguments(temp);
		temp2 = tmp->redirs;
		check_redirs(temp2);
		tmp = tmp->next;
	}
	return (cmd_list);
}