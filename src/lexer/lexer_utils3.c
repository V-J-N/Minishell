/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:45:46 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 17:31:19 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is a quote symbol and returs the corresponding
 * state.
 * This function serves as a low-level utility to identify a quote character.
 * @param c The character from the input string to be checked.
 * @return The corresponding 't_token_quote' state if the character is a quote.
 * Returns NONE otherwise.
 */
t_token_quote	verify_quotes(char c)
{
	t_token_quote	state;

	state = NONE;
	if (c == 34)
		return (state = DOUBLE);
	else if (c == 39)
		return (state = SINGLE);
	return (state);
}

/**
 * @brief Creates a final WORD token from the buffer and frees the buffer memory.
 * This function serves as the critical final step for anny accumulated word.
 * @param buffer A pointer to the dynamically allocated string containig the
 * accumulated word.
 * @param new_token A temporary pointer to hold the newly created token.
 * @param list A pointer to a pointer to the head of the main token list,
 * allowing the function to append the new token.
 * @return NULL the value of the now-freed and nulled buffer pointer.
 */
char	*tokenize_buffer(char *buffer, t_token *new_token, t_token **list)
{
	new_token = lstnew_token(buffer, WORD, NONE);
	if (!new_token)
		return (NULL);
	lstaddback_token(list, new_token);
	free(buffer);
	buffer = NULL;
	return (buffer);
}
