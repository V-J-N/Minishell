/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:26:41 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/20 13:26:50 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	contains_dollar(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 36)
			return (true);
		i++;
	}
	return (false);
}