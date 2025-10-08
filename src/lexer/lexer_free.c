/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:09:48 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/07 13:29:42 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Safely deallocates the 't_lexer' state structure and it contents.
 * @param lexer A pointer to the 't_lexer' structure to be deallocated.
 * @note This function must be called by 'advance_tokenizer' or any function
 * that handles the state structure's lifecycle. It does not touch the token
 * list (lexer->list), as that structure is returned to the caller for the
 * subsequent parsing phase.
 */
void	free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->buffer)
		free(lexer->buffer);
	free(lexer);
}
