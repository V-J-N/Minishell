/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:09:48 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 10:10:06 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * 
 */
void	free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->buffer)
		free(lexer->buffer);
	free(lexer);
}
