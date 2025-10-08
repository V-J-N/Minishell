/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:52 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/07 13:07:53 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * 
 */
#include "minishell.h"

/**
 * @brief Adds the current character to the lexer's word buffer and manages
 * dynamic resizing.
 * This function is called when 'check_none' identifies the current character
 * as part of a regular word.
 * It checks if the next character would exceed the currently allocated buffer
 * size, if resizing is necessary, it doubles the buffer size and calls
 * 'ft_realloc' to safely expand the memory block.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure.
 * Returns NULL if memory reallocation fails, indicating a fatal error.
 */
t_lexer	*tokenize_char(t_lexer *lexer)
{
	lexer->buffer[lexer->j] = lexer->string[lexer->i];
	lexer->i++;
	lexer->j++;
	if (lexer->j == lexer->buffer_size - 1)
	{
		lexer->buffer_size *= 2;
		lexer->buffer = ft_realloc(lexer->buffer, lexer->buffer_size);
		if (!lexer->buffer)
			return (free_tokens(&(lexer->list)), free_lexer(lexer), NULL);
	}
	lexer->buffer[lexer->j] = '\0';
	return (lexer);
}

/**
 * @brief Finalizes the current word token upon encountering a space and
 * advances the input index.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure.
 * Returns NULL if 'tokenize_buffer' fails due to a memory allocation error.
 */
t_lexer	*tokenize_space(t_lexer *lexer)
{
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	lexer->i++;
	return (lexer);
}

/**
 * @brief Finalizes the current word (if any) and creates a new PIPE token.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure.
 * Returns NULL if memory allocation fails during token creation or word
 * finalization.
 */
t_lexer	*tokenize_pipe(t_lexer *lexer)
{
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	lexer->new_token = lstnew_token("|", PIPE, lexer->quote);
	lstaddback_token(&(lexer->list), lexer->new_token);
	lexer->i++;
	return (lexer);
}

/**
 * @brief Finalizes the current word and creates either a REDIR_IN or
 * HEREDOC token.
 * This function handles the logic for input redirection operators. It
 * checks the character immediately following the current position to
 * determine the correct token type.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure.
 * Returns NULL if memory allocation fails during token creation or word
 * finalization.
 */
t_lexer	*tokenize_input(t_lexer *lexer)
{
	if (lexer->string[lexer->i + 1] == '<')
	{
		if (lexer->buffer && *lexer->buffer != '\0')
			lexer->buffer = tokenize_buffer(lexer->buffer,
					lexer->new_token, &(lexer->list));
		lexer->new_token = lstnew_token("<<", HEREDOC, lexer->quote);
		lstaddback_token(&(lexer->list), lexer->new_token);
		lexer->i += 2;
	}
	else
	{
		if (lexer->buffer && *lexer->buffer != '\0')
			lexer->buffer = tokenize_buffer(lexer->buffer,
					lexer->new_token, &(lexer->list));
		lexer->new_token = lstnew_token("<", REDIR_IN, lexer->quote);
		lstaddback_token(&(lexer->list), lexer->new_token);
		lexer->i++;
	}
	return (lexer);
}

/**
 * @brief Finalizes the current word and creates either a REDIR_OUT or
 * APPEND token.
 * This function handles the logic for output redirection operators. It checks
 * the character immediately following the current position to determine the
 * correct token type.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure.
 * Returns NULL if memory allocation fails during token creation or word
 * finalization.
 */
t_lexer	*tokenize_output(t_lexer *lexer)
{
	if (lexer->string[lexer->i + 1] == '>')
	{
		if (lexer->buffer && *lexer->buffer != '\0')
			lexer->buffer = tokenize_buffer(lexer->buffer,
					lexer->new_token, &(lexer->list));
		lexer->new_token = lstnew_token(">>", APPEND, lexer->quote);
		lstaddback_token(&(lexer->list), lexer->new_token);
		lexer->i += 2;
	}
	else
	{
		if (lexer->buffer && *lexer->buffer != '\0')
			lexer->buffer = tokenize_buffer(lexer->buffer,
					lexer->new_token, &(lexer->list));
		lexer->new_token = lstnew_token(">", REDIR_OUT, lexer->quote);
		lstaddback_token(&(lexer->list), lexer->new_token);
		lexer->i++;
	}
	return (lexer);
}
