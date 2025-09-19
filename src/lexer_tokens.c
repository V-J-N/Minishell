/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/19 15:30:09 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_quote	verify_quotes(char c)
{
	t_token_quote	state;

	state = NONE;
	if (c == 34)
		return (state = DOUBLE);
	else if (c == 39)
		return (state = SINGLE);
	return (state);
}

t_token	*advance_tokenizer(char *str)
{
	t_token			*list;
	t_token			*new_token;
	t_token_quote	quote;
	char			*buffer;
	size_t			i;
	size_t			j;
	size_t			buffer_size;

	i = 0;
	list = NULL;
	quote = NONE;
	buffer_size = 16;
	while (str[i])
	{
		buffer = ft_calloc(buffer_size, sizeof(char));
		j = 0;
		quote = verify_quotes(str[i]);
		if (quote == NONE)
		{
			while ((str[i] && ft_isalnum(str[i])) || str[i] == '-')
			{
				buffer[j] = str[i];
				i++;
				j++;
				if (j == buffer_size - 1)
				{
					buffer_size *=2;
					buffer = ft_realloc(buffer, buffer_size);
				}
			}
			if (str[i] == 32|| !str[i])
			{
				new_token = lstnew_token(buffer, WORD, quote);
				lstaddback_token(&list, new_token);
				if (!str[i])
				{
					free(buffer);
					break ;
				}
			}
			if (str[i] == '|')
			{
				new_token = lstnew_token("|", PIPE, quote);
				lstaddback_token(&list, new_token);
				i++;
			}
		}
		free(buffer);
		i++;
	}
	return (list);
}
