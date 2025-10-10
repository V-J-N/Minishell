/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:45:46 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/10 14:14:52 by serjimen         ###   ########.fr       */
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
	t_token_quote	quote;

	quote = NONE;
	if (c == 34)
		return (quote = DOUBLE);
	else if (c == 39)
		return (quote = SINGLE);
	return (quote);
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
char	*tokenize_buffer(char *buffer, t_token *new_token, t_token **list, t_token_quote quote)
{
	new_token = lstnew_token(buffer, WORD, quote);
	if (!new_token)
		return (NULL);
	lstaddback_token(list, new_token);
	free(buffer);
	buffer = NULL;
	return (buffer);
}

/**
 * @brief Allocates and initializes the lexer's word accumulation buffer.
 * This functions serves as the constructor for the dynamic buffer used to
 * build a word token char-by-char.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @returns The updated 't_lexer' state structure with a newly allocated buffer.
 * Returns NULL if memory allocations fails.
 */
t_lexer	*init_lexer_buffer(t_lexer *lexer)
{
	lexer->buffer = ft_calloc(lexer->buffer_size, sizeof(char));
	if (!lexer->buffer)
		return (NULL);
	lexer->j = 0;
	return (lexer);
}

/**
 * @brief Checks if the current character is a regular word character.
 * This function evaluates the character at the lexer's current position
 * (lexer->i) to see if it is a character that should be included in
 * a word token.
 * @param lexer A pointer to the 't_lexer' state structure, containing the
 * input string and the current position.
 * @return true if the character is a word character.
 * @return false if it is a metacharacter or a whitespace.
 */
bool	is_char(t_lexer *lexer)
{
	if (lexer->string[lexer->i] != 32 && lexer->string[lexer->i] != '|'
		&& lexer->string[lexer->i] != '<' && lexer->string[lexer->i] != '>')
		return (true);
	return (false);
}
