/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:04:32 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/07 18:57:58 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_state	set_state(t_token_quote quote, t_token_state state)
{
	if (state == OUT && quote == SINGLE)
		state = IN_SINGLE;
	else if (state == IN_SINGLE && quote == SINGLE)
		state = OUT;
	return (state);
}
