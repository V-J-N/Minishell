/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:02:17 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 12:03:18 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *nptr)
{
	long	integer;
	long	i;
	long	pos_neg;

	integer = 0;
	pos_neg = 1;
	i = 0;
	while (nptr[i] == 32 || (nptr[i] > 8 && nptr[i] < 14))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			pos_neg = -1;
		i++;
	}
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		integer = integer * 10 + (nptr[i] - '0');
		i++;
	}
	if (nptr[i] != '\0')
		return (0);
	return (integer * pos_neg);
}
