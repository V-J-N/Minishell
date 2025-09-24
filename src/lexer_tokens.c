/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/24 12:24:30 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (lexer);
}

t_token	*advance_tokenizer(char *str)
{
	t_lexer	*lexer;

	lexer = init_lexer(str);
	lexer = lexer_loop(lexer);
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	return (lexer->list);
}
