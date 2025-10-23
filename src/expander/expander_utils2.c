/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:53:48 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/23 10:43:25 by vjan-nie         ###   ########.fr       */
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
	return (arg->exp_value);
}

char	*redir_literal(t_redir *redir, size_t i)
{
	char	*old;
	char	*new;

	old = redir->exp_file;
	new = ft_calloc(2, sizeof(char));
	new[0] = '$';
	new[1] = '\0';
	redir->exp_file = ft_strjoin(old, new);
	free(old);
	free(new);
	while (redir->file[i] != 34)
	{
		redir_value(redir, i);
		i++;
	}
	return (redir->exp_file);
}
