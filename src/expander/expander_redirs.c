/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:26:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/10/23 10:42:24 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_file_double(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	temp = redirs;
	while (temp->file[i])
	{
		while (temp->file[i] == 34)
			i++;
		if (temp->file[i] == '\0')
			break ;
		temp->exp_file[j] = temp->file[i];
		i++;
		j++;
	}
	temp->exp_file[j] = '\0';
}

/**
 * 
 */
static void	copy_file_single(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	temp = redirs;
	while (temp->file[i])
	{
		while (temp->file[i] == 39)
			i++;
		if (temp->file[i] == '\0')
			break ;
		temp->exp_file[j] = temp->file[i];
		i++;
		j++;
	}
	temp->exp_file[j] = '\0';
}

t_redir	*change_file(t_redir *redirs)
{
	t_redir	*temp;

	temp = redirs;
	free(temp->file);
	temp->file = ft_strdup(temp->exp_file);
	free(temp->exp_file);
	temp->exp_file = NULL;
	return (temp);
}

t_redir	*check_doubles_redir(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	temp = redirs;
	len = ft_strlen(temp->file);
	while (temp->file[i])
	{
		if (temp->file[i] == 34)
			quotes++;
		i++;
	}
	temp->exp_file = malloc((len - quotes + 1) * sizeof(char));
	if (!temp->exp_file)
		return (NULL);
	copy_file_double(temp);
	return (temp);
}

/**
 * 
 */
t_redir	*check_singles_redir(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	temp = redirs;
	len = ft_strlen(temp->file);
	while (temp->file[i])
	{
		if (temp->file[i] == 39)
			quotes++;
		i++;
	}
	temp->exp_file = malloc((len - quotes + 1) * sizeof(char));
	if (!temp->exp_file)
		return (NULL);
	copy_file_single(temp);
	return (temp);
}
