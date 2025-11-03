/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:25:55 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 22:50:11 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_args.c
 * @brief Handles the final stage of argument processing: calculating the
 * final string length and removing the surrounding quotes.
 */
#include "minishell.h"

/**
 * @brief Copies the argument value into the expanded value (`exp_value`),
 * removing any double quotes (ASCII 34).
 * This function is used after variable expansion has already occurred.
 * @param args The argument node (t_arg) to process.
 */
static void	copy_value_double(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!args || !args->value)
		return ;
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
 * @brief Copies the argument value into the expanded value (`exp_value`),
 * removing any single quotes (ASCII 39).
 * Single quotes block expansion, so the value should already be stable
 * at this point.
 * @param args The argument node (t_arg) to process.
 */
static void	copy_value_single(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!args || !args->value)
		return ;
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
 * @brief Finalizes the argument value change. It frees the old value,
 * duplicates the final processed string from `exp_value` into `value`,
 * and frees `exp_value`.
 * This function consolidates the result of expansion and quote stripping.
 * @param args The argument node (t_arg) containing the final string in
 * `exp_value`.
 * @return t_arg* Returns the same argument node (`temp`).
 */
t_arg	*change_value(t_arg *args)
{
	t_arg	*temp;

	if (!args || !args->value)
		return (NULL);
	temp = args;
	free(temp->value);
	temp->value = ft_strdup(temp->exp_value);
	free(temp->exp_value);
	temp->exp_value = NULL;
	return (temp);
}

/**
 * @brief Calculates the length of the string after removing double quotes,
 * allocates memory for the new string (`exp_value`), and performs the copy.
 * This is the primary function for stripping double quotes from an argument.
 * @param args The argument node to check.
 * @return t_arg* The processed argument node, or NULL on memory allocation
 * failure.
 */
t_arg	*check_doubles_arg(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	if (!args || !args->value)
		return (NULL);
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
 * @brief Calculates the length of the string after removing single quotes,
 * allocates memory for the new string (`exp_value`), and performs the copy.
 * This is the primary function for stripping single quotes from an argument.
 * @param args The argument node to check.
 * @return t_arg* The processed argument node, or NULL on memory allocation
 * failure.
 */
t_arg	*check_singles_arg(t_arg *args)
{
	t_arg	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	if (!args || !args->value)
		return (NULL);
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
