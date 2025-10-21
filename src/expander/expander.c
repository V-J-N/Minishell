/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:24:54 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/21 12:00:17 by serjimen         ###   ########.fr       */
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
t_command *expander(t_command *cmd_list, t_env *env, int exit_signal)
{
	t_command	*tmp;
	t_arg		*temp;
	t_redir		*temp2;

	(void)env;
	tmp = cmd_list;
	while (tmp)
	{
		temp = tmp->args;
		while (temp)
		{
			if (temp->is_expanded && contains_dollar(temp->value))
			{
				size_t	i;

				i = 0;
				temp->exp_value = ft_strdup("");
				if (!temp->exp_value)
					return (NULL);
				while (temp->value[i])
				{
					if (temp->value[i] == 36)
					{
						i++;
						if (ft_isdigit(temp->value[i]))
						i++;
						else if (temp->value[i] == 63)
						{
							char	*old;
							char	*new;
							
							old = temp->exp_value;
							new = ft_strdup(ft_itoa(exit_signal));
							temp->exp_value = ft_strjoin(old, new);
							free(old);
							old = NULL;
							free(new);
							new = NULL;
							i++;
						}
						temp->env_value = ft_strdup("");
						if (!temp->env_value)
							return (NULL);
						while (ft_isalnum(temp->value[i]) || temp->value[i] == '_')
						{
							char	*old;
							char	*new;

							old = temp->env_value;
							new = ft_calloc(2, sizeof(char));
							new[0] = temp->value[i];
							i++;
							temp->env_value = ft_strjoin(old, new);
							free(old);
							old = NULL;
							free(new);
							new = NULL;
						}
						if (temp->env_value)
						{
							char	*old;
							char	*new;

							old = temp->exp_value;
							new = get_value_by_key(env, temp->env_value);
							temp->exp_value = ft_strjoin(old, new);
							free(old);
							old = NULL;
							free(new);
							new = NULL;
							free(temp->env_value);
							temp->env_value = NULL;
						}
					}
					else
					{
						char	*old;
						char	*new;

						old = temp->exp_value;
						new = ft_calloc(2, sizeof(char));
						new[0] = temp->value[i];
						i++;
						temp->exp_value = ft_strjoin(old, new);
						free(old);
						old = NULL;
						free(new);
						new = NULL;
					}
				}
				temp->is_expanded = false;
				if (temp->exp_value)
					change_value(temp);
			}
			temp = temp->next;
		}
		temp = tmp->args;
		check_arguments(temp);
		temp2 = tmp->redirs;
		check_redirs(temp2);
		tmp = tmp->next;
	}
	return (cmd_list);
}