/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:36:31 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/22 13:44:21 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*redir_exit(t_redir *redir, int exit)
{
	char	*old;
	char	*new;

	old = redir->exp_file;
	new = ft_strdup(ft_itoa(exit));
	redir->exp_file = ft_strjoin(old, new);
	free(old);
	free(new);
	redir->i++;
	return (redir->exp_file);
}

char	*redir_value(t_redir *redir, size_t i)
{
	char	*old;
	char	*new;

	old = redir->exp_file;
	new = ft_calloc(2, sizeof(char));
	new[0] = redir->file[i];
	redir->exp_file = ft_strjoin(old, new);
	free(old);
	free(new);
	redir->i++;
	return (redir->env_file);
}

char	*redir_environment(t_redir *redir, size_t i)
{
	char	*old;
	char	*new;

	old = redir->env_file;
	new = ft_calloc(2, sizeof(char));
	new[0] = redir->file[i];
	i++;
	redir->env_file = ft_strjoin(old, new);
	free(old);
	free(new);
	redir->i++;
	return(redir->env_file);
}

char	*get_expand_redir(t_redir *redir, t_env *env)
{
	char	*old;
	char	*new;

	old = redir->exp_file;
	new = get_value_by_key(env, redir->env_file);
	redir->exp_file = ft_strjoin(old, new);
	free(old);
	free(new);
	free(redir->env_file);
	redir->env_file = NULL;
	return (redir->exp_file);
}
