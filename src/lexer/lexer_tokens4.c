/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:55:38 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/22 10:12:45 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_tokens4.c
 * @brief State handler functions for character processing.
 */
#include "minishell.h"

/**
 * @brief Dispatches lexer logic based on the current character in the unquoted
 * state.
 * This function is the primary control flow for the lexer when the 'quote'
 * state in OUT.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure upon succesful processing.
 * Returns NULL if a memory allocation error occurs.
 * @note This handler is unique because it must identify and process all
 * metacharacters and whitespaces.
 */
t_lexer	*check_none(t_lexer *lexer)
{
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	if (is_char(lexer))
	{
		lexer = tokenize_char(lexer);
		if (!lexer)
			return (NULL);
	}
	else if (lexer->string[lexer->i] == ' ')
		lexer = tokenize_space(lexer);
	else if (lexer->string[lexer->i] == '|')
		lexer = tokenize_pipe(lexer);
	else if (lexer->string[lexer->i] == '>')
		lexer = tokenize_output(lexer);
	else if (lexer->string[lexer->i] == '<')
		lexer = tokenize_input(lexer);
	if (!lexer)
		return (NULL);
	return (lexer);
}

/**
 * @brief Processes characters when the lexer is inside single quotes.
 * This function implements the tokenization logic fot the 'IN_SINGLE' state.
 * Inside single quotes, all character are treated as literal data and are
 * accumulated directly into the word buffer, woth the exception of the closing
 * single quote.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure upon successful processing.
 * Returns NULL if a memory allocation error occurs.
 */
t_lexer	*check_single(t_lexer *lexer)
{
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	if (lexer->string[lexer->i] != '\0')
	{
		lexer = tokenize_char(lexer);
		if (!lexer)
			return (NULL);
	}
	else
		lexer->i++;
	if (!lexer)
		return (NULL);
	return (lexer);
}

/**
 * @brief Processes characters when the lexer is inside double quotes.
 * This function implements the tokenization logic for the 'IN_DOUBLE' state.
 * Inside double quotes, characters are treated as literal data, except for
 * the closing double quote and potential variable expansion symbols.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure upon successful processing.
 * Returns NULL if a memory allocation error occurs.
 */
t_lexer	*check_double(t_lexer *lexer)
{
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	if (lexer->string[lexer->i] != '\0')
	{
		lexer = tokenize_char(lexer);
		if (!lexer)
			return (NULL);
	}
	else
		lexer->i++;
	if (!lexer)
		return (NULL);
	return (lexer);
}
