/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:22:23 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/15 19:29:24 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 *
 */
#include "minishell.h"

t_command *expander(t_command *cmd_list)
{
	t_command *tmp;
	t_arg *temp;
	t_redir *temp2;
	size_t i;
	size_t j;
	size_t quotes;
	size_t len;

	tmp = cmd_list;
	while (tmp)
	{
		temp = tmp->args;
		while (temp)
		{
			i = 0;
			j = 0;
			quotes = 0;
			len = ft_strlen(temp->value);
			if (temp->has_quotes)
			{
				if (temp->quote == NONE || temp->quote == SINGLE)
				{
					while (temp->value[i])
					{
						if (temp->value[i] == 39)
						{
							quotes++;
						}
						i++;
					}
					temp->exp_value = ft_calloc(1, (len - quotes + 1));
					i = 0;
					while (temp->value[i])
					{
						while (temp->value[i] == 39)
						{
							i++;
							if (temp->value[i] == '\0')
								break;
						}
						temp->exp_value[j] = temp->value[i];
						i++;
						j++;
					}
					temp->exp_value[j] = '\0';
				}
			}
			if (temp->exp_value != NULL)
			{
				free(temp->value);
				temp->value = ft_strdup(temp->exp_value);
				free(temp->exp_value);
				temp->exp_value = NULL;
			}
			temp = temp->next;
		}
		temp2 = tmp->redirs;
		while (temp2)
		{
			i = 0;
			j = 0;
			quotes = 0;
			len = ft_strlen(temp2->file);
			if (temp2->has_quotes)
			{
				if (temp2->quote == NONE || temp2->quote == SINGLE)
				{
					while (temp2->file[i])
					{
						if (temp2->file[i] == 39)
						{
							quotes++;
						}
						i++;
					}
					temp2->exp_file = ft_calloc(1, (len - quotes + 1));
					i = 0;
					while (temp2->file[i])
					{
						while (temp2->file[i] == 39)
						{
							i++;
							if (temp2->file[i] == '\0')
								break;
						}
						temp2->exp_file[j] = temp2->file[i];
						i++;
						j++;
					}
					temp2->exp_file[j] = '\0';
				}
			}
			if (temp2->exp_file != NULL)
			{
				free(temp2->file);
				temp2->file = ft_strdup(temp2->exp_file);
				free(temp2->exp_file);
				temp2->exp_file = NULL;
			}
			temp2 = temp2->next;
		}
		tmp = tmp->next;
	}
	return (cmd_list);
}
