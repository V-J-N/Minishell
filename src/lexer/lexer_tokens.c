/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 11:31:34 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer_tokens.c
 * @brief Core logic for the character-by-character tokenization.
 */
#include "minishell.h"

/**
 * @brief Allocates and initializes the lexer state structure.
 * This function is the constructor for the 't_lexer' state structure. It
 * allocates memory for the structure and sets all its fields to a clean,
 * defined starting state.
 * @param str The raw command-line string that the lexer will process.
 * @returns A pointer to the newly allocated and initialized 't_lexer'
 * structure. Returns NULL if memory allocation fails.
 */
t_lexer	*init_lexer(char *str)
{
	t_lexer	*lexer;

	lexer = ft_calloc(1, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->list = NULL;
	lexer->new_token = NULL;
	lexer->quote = NONE;
	lexer->buffer = NULL;
	lexer->string = str;
	lexer->i = 0;
	lexer->j = 0;
	lexer->buffer_size = 16;
	lexer->state = OUT;
	lexer->has_quotes = false;
	return (lexer);
}

/**
 * @brief Performs char-by-char tokenization for the input string.
 * This function implements the core lexing algorithm. It converts th raw
 * input string into a structured linked list of tokens in four steps.
 * 1 - State Initialization: It calls 'init_lexer' to allocate and initialize
 * the 't_lexer' state structure. This structure holds all necessary parsing
 * metadata.
 * 2 - Iterative Scanning: It calls 'lexer_loop' to perform the main character
 * scanning. This loop typically acts as a state machine, reading chars,
 * accumulating a word buffer, handling quotes, and identifying metacharacters
 * to build partial tokens.
 * 3 - Final Tokenization: After the loops finishes, it checks if the buffer
 * still holds data and append it as the final token to the list.
 * 4 - Cleanup: Stores the generated token list and calls 'free_lexer' to
 * safely deallocate the 't_lexer' structure and its internal buffers.
 * @param str The raw command-line string received from 'tokenizer'.
 * @return A pointer to the head of the newly created 't_token' linked list.
 * Returns NULL if initialization fails or if a memory allocation fails.
 * @note This function ist desing using a dedicated state structure 't_lexer'
 * simplifies the passing state information between different processing
 * functions.
 */
t_token	*advance_tokenizer(char *str)
{
	t_lexer	*lexer;
	t_token	*tokens;

	tokens = NULL;
	lexer = init_lexer(str);
	if (!lexer)
		return (NULL);
	lexer = lexer_loop(lexer);
	if (!lexer)
		return (NULL);
	if (lexer->buffer && *lexer->buffer != '\0')
	{
		if (lexer->i > 0)
			lexer->quote = verify_quotes(lexer->string[lexer->i-1]);
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list), lexer->quote,
					lexer->has_quotes);
	}
	if (lexer->state != OUT)
		return (free_tokens(&(lexer->list)), free_lexer(lexer), NULL);
	tokens = lexer->list;
	free_lexer(lexer);
	return (tokens);
}
