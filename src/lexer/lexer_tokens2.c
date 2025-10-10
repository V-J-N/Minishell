/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:46:25 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/10 13:19:59 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Drives the main character scanning loop for tokenization.
 * This function iterates over the input string char-by-char.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' structure upon succesful completion of
 * the loop. Returns NULL if the input state was invalid or if a fatal
 * error occurred during processing.
 * @note This loop is the state machine's heart. Its primary responsibility
 * is to determine the context (quoted or unquoted).
 */
t_lexer	*lexer_loop(t_lexer *lexer)
{
	if (!lexer)
		return (NULL);
	while (lexer->string[lexer->i])
	{
		lexer->quote = verify_quotes(lexer->string[lexer->i]);
		lexer->state = set_state(lexer->quote, lexer->state);
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
	}
	return (lexer);
}
