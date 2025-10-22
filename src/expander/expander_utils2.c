/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:53:48 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/22 11:41:11 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_literal(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = ft_calloc(2, sizeof(char));
	new[0] = '$';
	new[1] = '\0';
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	while (arg->value[i] != 34)
	{
		append_value(arg, i);
		i++;
	}
	return(arg->exp_value);
}
