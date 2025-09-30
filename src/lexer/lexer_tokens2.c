/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:46:25 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 18:50:31 by serjimen         ###   ########.fr       */
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
	lexer->quote = verify_quotes(lexer->string[lexer->i]);
	while (lexer->string[lexer->i])
	{
		if (lexer->quote == NONE)
		{
			lexer = check_none(lexer);
			if (!lexer)
				return (NULL);
		}
		else if (lexer->quote == SINGLE)
		{
			if (lexer->is_open == false)
				lexer->is_open = true;
			else
				lexer->is_open = false;
			lexer = check_single(lexer);
			if (!lexer)
				return (NULL);
		}
	}
	return (lexer);
}
