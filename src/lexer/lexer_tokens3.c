/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:52 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/24 12:24:03 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (free_tokens(&(lexer->list)), NULL);
	}
	lexer->buffer[lexer->j] = '\0';
	return (lexer);
}

t_lexer	*tokenize_space(t_lexer *lexer)
{
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	lexer->i++;
	return (lexer);
}

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
