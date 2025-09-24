/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:45:46 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/24 12:25:21 by serjimen         ###   ########.fr       */
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

char	*tokenize_buffer(char *buffer, t_token *new_token, t_token **list)
{
	new_token = lstnew_token(buffer, WORD, NONE);
	lstaddback_token(list, new_token);
	free(buffer);
	buffer = NULL;
	return (buffer);
}
