/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:46:25 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/24 12:22:43 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_char(t_lexer *lexer)
{
	if (lexer->string[lexer->i] != 32 && lexer->string[lexer->i] != '|'
		&& lexer->string[lexer->i] != '<' && lexer->string[lexer->i] != '>')
		return (true);
	return (false);
}

static t_lexer	*init_lexer_buffer(t_lexer *lexer)
{
	lexer->buffer = ft_calloc(lexer->buffer_size, sizeof(char));
	if (!lexer->buffer)
		return (NULL);
	lexer->j = 0;
	return (lexer);
}

static t_lexer	*check_none(t_lexer *lexer)
{
	if (lexer->buffer == NULL)
		lexer = init_lexer_buffer(lexer);
	if (is_char(lexer))
		tokenize_char(lexer);
	else if (lexer->string[lexer->i] == 32)
		tokenize_space(lexer);
	else if (lexer->string[lexer->i] == '|')
		tokenize_pipe(lexer);
	else if (lexer->string[lexer->i] == '>')
		tokenize_output(lexer);
	else if (lexer->string[lexer->i] == '<')
		tokenize_input(lexer);
	return (lexer);
}

t_lexer	*lexer_loop(t_lexer *lexer)
{
	while (lexer->string[lexer->i])
	{
		lexer->quote = verify_quotes(lexer->string[lexer->i]);
		if (lexer->quote == NONE)
			lexer = check_none(lexer);
	}
	return (lexer);
}
