/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redirs2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:25:28 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/22 13:44:16 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	redir_dollar(t_redir *redir, t_env *env, int exit)
{
	if (redir->file[redir->i] == 36)
	{
		redir->i++;
		if (ft_isdigit(redir->file[redir->i]))
			redir->i++;
		else if (redir->file[redir->i] == 63)
			redir_exit(redir, exit);
		else if (redir->file[redir->i] == 34)
			redir_literal(redir, redir->i);
		redir->env_file = ft_strdup("");
		if (!redir->env_file)
			return ;
		while (ft_isalnum(redir->file[redir->i]) || redir->file[redir->i] == '_')
			redir_environment(redir, redir->i);
		if (redir->env_file)
			get_expand_redir(redir, env);
	}
	else
		redir_value(redir, redir->i);
}

t_redir	*expand_redirs(t_redir *redir, t_env *env, int exit)
{
	t_redir	*temp;

	temp = redir;
	while (temp)
	{
		if (temp->is_expanded && contains_dollar(temp->file))
		{
			temp->exp_file = ft_strdup("");
			if (!temp->exp_file)
				return (NULL);
			while (temp->file[temp->i])
				redir_dollar(temp, env, exit);
			temp->is_expanded = false;
			if (temp->exp_file)
				change_file(temp);
		}
		temp = temp->next;
	}
	return (temp);
}
