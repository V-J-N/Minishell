/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:04:32 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/11 09:27:34 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * 
 */
bool	ft_isupper(char c)
{
	if (c <= 'A' && c >= 'Z')
		return (false);
	return (true);
}

/**
 * 
 */
t_token_state	set_state(t_token_quote quote, t_token_state state)
{
	if (state == OUT && quote == SINGLE)
		state = IN_SINGLE;
	else if (state == IN_SINGLE && quote == SINGLE)
		state = OUT;
	else if (state == OUT && quote == DOUBLE)
		state = IN_DOUBLE;
	else if (state == IN_DOUBLE && quote == DOUBLE)
		state = OUT;
	return (state);
}

/**
 * 
 */
t_lexer	*tokenize_env(t_lexer *lexer)
{
	while (1)
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
		if (lexer->string[lexer->i] == 32 || lexer->string[lexer->i] == 34)
			break ;
	}
	return (lexer);
}
