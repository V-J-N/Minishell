/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/25 11:04:16 by vjan-nie         ###   ########.fr       */
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

/* t_token	*advance_tokenizer(char *str)
{
	t_lexer	*lexer;

	lexer = init_lexer(str);
	lexer = lexer_loop(lexer);
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	return (lexer->list);
} */
//He encontrado un leak por aquí, así que me he puesto a arreglarlo
//en un momento con ayuda de la IA. Te he comentado la función original
//para que no se pierda, y hemos añadido una función helper.

void	free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->buffer)
		free(lexer->buffer);
	free(lexer);
}

t_token	*advance_tokenizer(char *str)
{
	t_lexer	*lexer;
	t_token	*tokens;

	lexer = init_lexer(str);
	lexer = lexer_loop(lexer);
	if (lexer->buffer && *lexer->buffer != '\0')
		lexer->buffer = tokenize_buffer(lexer->buffer,
				lexer->new_token, &(lexer->list));
	tokens = lexer->list;
	free_lexer(lexer);
	return (tokens);
}
