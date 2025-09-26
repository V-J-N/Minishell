/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:46:25 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 17:36:45 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
static bool	is_char(t_lexer *lexer)
{
	if (lexer->string[lexer->i] != 32 && lexer->string[lexer->i] != '|'
		&& lexer->string[lexer->i] != '<' && lexer->string[lexer->i] != '>')
		return (true);
	return (false);
}

/**
 * @brief Allocates and initializes the lexer's word accumulation buffer.
 * This functions serves as the constructor for the dynamic buffer used to
 * build a word token char-by-char.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @returns The updated 't_lexer' state structure with a newly allocated buffer.
 * Returns NULL if memory allocations fails.
 */
static t_lexer	*init_lexer_buffer(t_lexer *lexer)
{
	lexer->buffer = ft_calloc(lexer->buffer_size, sizeof(char));
	if (!lexer->buffer)
		return (NULL);
	lexer->j = 0;
	return (lexer);
}

/**
 * @brief Dispatches lexer logic based on the current character in th quoted
 * state.
 * This function is the primary control flow for the lexer when th 'quote' state
 * in NONE.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure upon succesful processing.
 * Returns NULL if a memory allocation error occurs.
 */
static t_lexer	*check_none(t_lexer *lexer)
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
			return NULL;
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
		if (lexer->quote == NONE)
		{
			lexer = check_none(lexer);
			if (!lexer)
				return (NULL);
		}
	}
	return (lexer);
}
