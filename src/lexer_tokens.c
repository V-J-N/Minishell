/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/18 13:12:08 by sergio-jime      ###   ########.fr       */
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
	size_t	i;

	i = 0;
	list = NULL;
	quote = NONE;
	buffer = ft_calloc(16, sizeof(char));
	while (str[i])
	{
		quote = verify_quotes(str[i]);
		if (quote == NONE)
		{
			while (ft_isalnum(str[i]) && str[i])
			{
				buffer[i] = str[i];
				i++;
				// if (i == 16) aumentar el buffer
			}
			if (str[i] == 32 || !str[i])
			{
				new_token = lstnew_token(buffer, WORD, quote);
				free(buffer);
				lstaddback_token(&list, new_token);
				free_tokens(&new_token);
				if (!str[i])
					break ;
			}
		}
		i++;
	}
	return (list);
}