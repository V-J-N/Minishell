/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:22:23 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/15 12:47:16 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * 
 */
#include "minishell.h"

t_command *expander(t_command *cmd_list)
{
	t_arg	*temp;
	size_t	i;
	size_t	j;
	size_t	quotes;
	size_t	len;

	temp = cmd_list->args;
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
							break ;
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
	return (cmd_list);
}
