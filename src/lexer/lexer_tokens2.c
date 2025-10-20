/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:46:25 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/20 16:20:44 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_tokens2.c
 * @brief Dispatcher for the lexing state machine.
 */
#include "minishell.h"

/**
 * @brief Dispatches character proccessing based on the lexer's current state.
 * This function is the core decision-maker, invoked during each iteration of
 * the 'lexer_loop'. It directs the flow to the appropiate handler routine
 * based on whether the lexer is outside of quotes or inside single or
 * double quotes.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updates 't_lexer' state structure. Returns NULL if a fatal error
 * occurs in any of the delegated handler functions.
 */
t_lexer	*lexer_selector(t_lexer *lexer)
{
	if (lexer->state != OUT)
		lexer->has_quotes = true;
	if (lexer->state == OUT)
	{
		lexer = check_none(lexer);
		if (!lexer)
			return (NULL);
	}
	else if (lexer->state == IN_SINGLE)
	{
		lexer = check_single(lexer);
		if (!lexer)
			return (NULL);
	}
	else if (lexer->state == IN_DOUBLE)
	{
		lexer = check_double(lexer);
		if (!lexer)
			return (NULL);
	}
	return (lexer);
}

/**
 * @brief Drives the main character scanning loop for tokenization.
 * This function iterates over the input string char-by-char.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' structure upon succesful completion of
 * the loop. Returns NULL if the input state was invalid or if a fatal
 * error occurred during processing.
 * @note This loop is the state machine's heart. Its primary responsibility
 * is to manage the state transitions and delegate the complex character
 * handling logic to maintain modularity.
 */
t_lexer	*lexer_loop(t_lexer *lexer)
{
	if (!lexer)
		return (NULL);
	while (lexer->string[lexer->i])
	{
		if (((lexer->string[lexer->i] == 34 && lexer->string[lexer->i + 1] == 34))
			|| ((lexer->string[lexer->i] == 39 && lexer->string[lexer->i + 1] == 39)))
		{
			lexer->i += 2;
		}
		lexer->quote = verify_quotes(lexer->string[lexer->i]);
		lexer->state = set_state(lexer->quote, lexer->state);
		lexer_selector(lexer);
		if (!lexer)
			return (NULL);
	}
	return (lexer);
}
