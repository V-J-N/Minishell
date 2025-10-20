/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:25:55 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/20 13:26:09 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 *
 */
#include "minishell.h"

/**
 * 
 */
static void copy_value_double(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	temp = args;
	while (temp->value[i])
	{
		while (temp->value[i] == 34)
			i++;
		if (temp->value[i] == '\0')
			break ;
		temp->exp_value[j] = temp->value[i];
		i++;
		j++;
	}
	temp->exp_value[j] = '\0';
}

/**
 * 
 */
static void copy_value_single(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	temp = args;
	while (temp->value[i])
	{
		while (temp->value[i] == 39)
			i++;
		if (temp->value[i] == '\0')
			break ;
		temp->exp_value[j] = temp->value[i];
		i++;
		j++;
	}
	temp->exp_value[j] = '\0';
}

/**
 *
 */
t_arg *change_value(t_arg *args)
{
	t_arg *temp;

	temp = args;
	free(temp->value);
	temp->value = ft_strdup(temp->exp_value);
	free(temp->exp_value);
	temp->exp_value = NULL;
	return (temp);
}

/**
 *
 */
t_arg *check_doubles_arg(t_arg *args)
{
	t_arg *temp;
	size_t i;
	size_t quotes;
	size_t len;

	i = 0;
	quotes = 0;
	temp = args;
	len = ft_strlen(temp->value);
	while (temp->value[i])
	{
		if (temp->value[i] == 34)
			quotes++;
		i++;
	}
	temp->exp_value = malloc((len - quotes + 1) * sizeof(char));
	if (!temp->exp_value)
		return (NULL);
	copy_value_double(temp);
	return (temp);
}

/**
 *
 */
t_arg *check_singles_arg(t_arg *args)
{
	t_arg *temp;
	size_t i;
	size_t quotes;
	size_t len;

	i = 0;
	quotes = 0;
	temp = args;
	len = ft_strlen(temp->value);
	while (temp->value[i])
	{
		if (temp->value[i] == 39)
			quotes++;
		i++;
	}
	temp->exp_value = malloc((len - quotes + 1) * sizeof(char));
	if (!temp->exp_value)
		return (NULL);
	copy_value_single(temp);
	return (temp);
}