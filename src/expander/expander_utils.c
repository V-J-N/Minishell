/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:26:41 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/21 13:12:49 by serjimen         ###   ########.fr       */
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

char	*append_value(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = ft_calloc(2, sizeof(char));
	new[0] = arg->value[i];
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	arg->i++;
	return (arg->env_value);
}

char	*append_environment(t_arg *arg, size_t i)
{
	char	*old;
	char	*new;

	old = arg->env_value;
	new = ft_calloc(2, sizeof(char));
	new[0] = arg->value[i];
	i++;
	arg->env_value = ft_strjoin(old, new);
	free(old);
	free(new);
	arg->i++;
	return(arg->env_value);
}

char	*get_expand_env(t_arg *arg, t_env *env)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = get_value_by_key(env, arg->env_value);
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	free(arg->env_value);
	arg->env_value = NULL;
	return (arg->exp_value);
}

char	*append_exit(t_arg *arg, int exit)
{
	char	*old;
	char	*new;

	old = arg->exp_value;
	new = ft_strdup(ft_itoa(exit));
	arg->exp_value = ft_strjoin(old, new);
	free(old);
	free(new);
	arg->i++;
	return (arg->exp_value);
}
