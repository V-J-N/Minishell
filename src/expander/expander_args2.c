/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_args2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 12:25:56 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/22 11:27:55 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_dollar(t_arg *arg, t_env *env, int exit)
{
	if (arg->value[arg->i] == 36)
	{
		arg->i++;
		if (ft_isdigit(arg->value[arg->i]))
			arg->i++;
		else if (arg->value[arg->i] == 63)
			append_exit(arg, exit);
		else if (arg->value[arg->i] == 34)
			append_literal(arg, arg->i);
		arg->env_value = ft_strdup("");
		if (!arg->env_value)
			return ;
		while (ft_isalnum(arg->value[arg->i]) || arg->value[arg->i] == '_')
			append_environment(arg, arg->i);
		if (arg->env_value)
			get_expand_env(arg, env);
	}
	else
		append_value(arg, arg->i);
}

t_arg	*expand_args(t_arg *args, t_env *env, int exit)
{
	t_arg	*temp;

	temp = args;
	while (temp)
	{
		if (temp->is_expanded && contains_dollar(temp->value))
		{
			temp->exp_value = ft_strdup("");
			if (!temp->exp_value)
				return (NULL);
			while (temp->value[temp->i])
				append_dollar(temp, env, exit);
			temp->is_expanded = false;
			if (temp->exp_value)
				change_value(temp);
		}
		temp = temp->next;
	}
	return (temp);
}
