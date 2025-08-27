/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 11:10:15 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/08/27 11:17:27 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_lstsize_token(t_token *lst)
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
 * OJO!: He cambiado la asignación a través de un ft_strdup(), porque si no
 depende de lo que haya en envlist ya que sólo asignamos puntero al contenido del nodo.
 Con strdup nos aseguramos de que tenemos un array independiente sobre el que luego
 podemos hacer free
 */
char	**tokenlist_to_arr(t_token **tokenlist)
{
	int		count;
	char	**tokenarr;
	t_token	*temp;
	int		i;

	count = ft_lstsize_token(*tokenlist);
	tokenarr = ft_calloc(count + 1, sizeof(char *));
	if (!tokenarr)
		return (NULL);
	temp = *tokenlist;
	i = 0;
	while (temp)
	{
		tokenarr[i] = ft_strdup(temp->value);
		temp = temp->next;
		i++;
	}
	tokenarr[i] = NULL;
	return (tokenarr);
}