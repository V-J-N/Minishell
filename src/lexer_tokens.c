/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:24:32 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/22 17:43:23 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tokenize_buffer(char *buffer, t_token *new_token, t_token **list)
{
		new_token = lstnew_token(buffer, WORD, NONE);
		lstaddback_token(list, new_token);
		free(buffer);
		buffer = NULL;
		return (buffer);
}

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
	buffer = NULL;
	new_token = NULL;
	while (str[i])
	{
		quote = verify_quotes(str[i]);
		if (quote == NONE)
		{
			if (buffer == NULL)
			{
				buffer = ft_calloc(buffer_size, sizeof(char));
				j = 0;
			}
			if (str[i] != 32 && str[i] != '|' && str[i] != '<' && str[i] != '>')
			{
				buffer[j] = str[i];
				i++;
				j++;
				if (j == buffer_size - 1)
				{
					buffer_size *=2;
					buffer = ft_realloc(buffer, buffer_size);
					if (!buffer)
						return (free_tokens(&list), NULL);
				}
				buffer[j] = '\0';
			}
			else if (str[i] == 32)
			{
				if (buffer && *buffer != '\0')
					buffer = tokenize_buffer(buffer, new_token, &list);
				i++;
			}
			else if (str[i] == '|')
			{
				if (buffer && *buffer != '\0')
					buffer = tokenize_buffer(buffer, new_token, &list);
				new_token = lstnew_token("|", PIPE, quote);
				lstaddback_token(&list, new_token);
				i++;
			}
			else if (str[i] == '>')
			{
				if (str[i+1] == '>')
				{
					if (buffer && *buffer != '\0')
						buffer = tokenize_buffer(buffer, new_token, &list);
					new_token = lstnew_token(">>", APPEND, quote);
					lstaddback_token(&list, new_token);
					i += 2;
				}
				else
				{
					if (buffer && *buffer != '\0')
						buffer = tokenize_buffer(buffer, new_token, &list);
					new_token = lstnew_token(">", REDIR_OUT, quote);
					lstaddback_token(&list, new_token);
					i++;
				}
			}
			else if (str[i] == '<')
			{
				if (str[i+1] == '<')
				{
					if (buffer && *buffer != '\0')
						buffer = tokenize_buffer(buffer, new_token, &list);
					new_token = lstnew_token("<<", HEREDOC, quote);
					lstaddback_token(&list, new_token);
					i += 2;
				}
				else
				{
					if (buffer && *buffer != '\0')
						buffer = tokenize_buffer(buffer, new_token, &list);
					new_token = lstnew_token("<", REDIR_IN, quote);
					lstaddback_token(&list, new_token);
					i++;
				}
			}
		}
	}
	if (buffer && *buffer != '\0')
		buffer = tokenize_buffer(buffer, new_token, &list);
	return (list);
}
