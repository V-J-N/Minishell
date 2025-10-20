/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:04:32 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/20 15:17:19 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_utils4.c
 * @brief Utilities for managing the lexer's quoting state.
 */
#include "minishell.h"

/**
 * @brief Checks if a character is a quote symbol and returs the corresponding
 * quote type.
 * This function serves as a low-level utility to identify a quote character.
 * @param c The character from the input string to be checked.
 * @return The corresponding 't_token_quote' type if the character is a quote.
 * Returns NONE otherwise.
 */
t_token_quote	verify_quotes(char c)
{
	t_token_quote	quote;

	quote = NONE;
	if (c == 34)
		return (quote = DOUBLE);
	else if (c == 39)
		return (quote = SINGLE);
	return (quote);
}

/**
 * @brief Implements the state transition logic for quoting.
 * This function is the core of the lexer's state machine logic. It determines
 * the new quoting state based on the current state and the type of quote
 * character encountered.
 * @param quote The type of quote character encountered.
 * @param state The current 't_token_state' of the lexer.
 * @return The new 't_token_state' after applying the transition logic.
 */
t_token_state	set_state(t_token_quote quote, t_token_state state)
{
	if (state == OUT && quote == SINGLE)
		state = IN_SINGLE;
	else if (state == IN_SINGLE && quote == SINGLE)
		state = OUT;
	else if (state == OUT && quote == DOUBLE)
		state = IN_DOUBLE;
	else if (state == IN_DOUBLE && quote == DOUBLE)
		state = OUT;
	return (state);
}

/**
 * 
 */
bool	check_quotes(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			return (true);
		i++;
	}
	return (false);
}
