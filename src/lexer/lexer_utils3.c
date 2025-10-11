/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:45:46 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/11 17:05:58 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_utils3.c
 * @brief Core utility functions for buffer management and word token
 * finalization.
 */
#include "minishell.h"

/**
 * @brief Creates a final WORD token from the buffer and frees the buffer memory.
 * This function serves as the critical final step for any accumulated word.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return NULL the value of the now-freed and nulled buffer pointer.
 * This value signals the caller that the buffer has been cleared and the lexer
 * state is ready for a new word.
 */
char	*tokenize_buffer(t_lexer *lexer)
{
	lexer->new_token = lstnew_token(lexer->buffer, WORD, lexer->quote,
			lexer->has_quotes);
	if (!lexer->new_token)
		return (NULL);
	lstaddback_token(&(lexer->list), lexer->new_token);
	free(lexer->buffer);
	lexer->buffer = NULL;
	return (lexer->buffer);
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
