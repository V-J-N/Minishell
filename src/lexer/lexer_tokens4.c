/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:55:38 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/11 10:34:20 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Dispatches lexer logic based on the current character in th quoted
 * state.
 * This function is the primary control flow for the lexer when th 'quote' state
 * in NONE.
 * @param lexer A pointer to the 't_lexer' state structure.
 * @return The updated 't_lexer' state structure upon succesful processing.
 * Returns NULL if a memory allocation error occurs.
 */
t_lexer	*check_none(t_lexer *lexer)
{
/* 	if (lexer->string[lexer->i] == 39)
	{
		if (lexer->string[lexer->i+1] == 39)
		{
			lexer->i++;
			return (lexer);
		}
		else if (ft_isprint(lexer->string[lexer->i+1]))
		{
			lexer->i++;
			lexer = tokenize_char(lexer);
			if (!lexer)
				return NULL;
		}
		else
		{
			lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list), SINGLE);
				lexer->i++;
		}
	}
	else if (lexer->string[lexer->i] == 34)
	{
		if (lexer->string[lexer->i+1] == 34)
		{
			lexer->i++;
			return (lexer);
		}
		else if (ft_isprint(lexer->string[lexer->i+1]))
		{
			lexer->i++;
			lexer = tokenize_char(lexer);
			if (!lexer)
				return NULL;
		}
		else
		{
			lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list), DOUBLE);
				lexer->i++;
		}
	} */
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	else if (is_char(lexer))
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
 * 
 */
t_lexer	*check_single(t_lexer *lexer)
{
/* 	if (lexer->string[lexer->i] == 39)
	{
		lexer->i++;
		if (lexer->string[lexer->i] == 39)
		{
			lexer->i++;
			lexer->state = OUT;
			return (lexer);
		}
	} */
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	if (ft_isprint(lexer->string[lexer->i]))
	{
		lexer = tokenize_char(lexer);
		if (!lexer)
			return (NULL);
	}
	if (!lexer)
		return (NULL);
	return (lexer);
}

/**
 * 
 */
t_lexer	*check_double(t_lexer *lexer)
{
/* 	if (lexer->string[lexer->i] == 34)
	{
		lexer->i++;
		if (lexer->string[lexer->i] == 34)
		{
			lexer->i++;
			lexer->state = OUT;
			return (lexer);
		}
	} */
	if (lexer->buffer == NULL)
	{
		lexer = init_lexer_buffer(lexer);
		if (!lexer)
			return (NULL);
	}
	if (ft_isprint(lexer->string[lexer->i]))
	{
		lexer = tokenize_char(lexer);
		if (!lexer)
			return (NULL);
	}
	if (!lexer)
		return (NULL);
	return (lexer);
}
