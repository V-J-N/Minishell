/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:25:00 by serjimen          #+#    #+#             */
/*   Updated: 2025/08/20 11:55:10 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_lstsize_mini(t_env *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}

/**
 * 
 */
char	**envlist_to_arr(t_env **envlist)
{
	int		count;
	char	**envarr;
	t_env	*temp;
	int		i;

	count = ft_lstsize_mini(*envlist);
	envarr = ft_calloc(count + 1, sizeof(char *));
	if (!envarr)
		return (NULL);
	temp = *envlist;
	i = 0;
	while (temp)
	{
		envarr[i] = temp->full_env;
		temp = temp->next;
		i++;
	}
	envarr[i] = NULL;
	return (envarr);
}
