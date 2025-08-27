/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:33:51 by serjimen          #+#    #+#             */
/*   Updated: 2025/08/27 11:03:56 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*set_token(char *str)
{
	t_token	*new_token;

	if (ft_strlen(str) == 2 && ft_strncmp(str, ">>", 2) == 0)
		new_token = ft_lstnew_token(str, APPEND);
	else if (ft_strlen(str) == 2 && ft_strncmp(str, "<<", 2) == 0)
		new_token = ft_lstnew_token(str, HEREDOC);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, ">", 1) == 0)
		new_token = ft_lstnew_token(str, REDIR_OUT);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, "<", 1) == 0)
		new_token = ft_lstnew_token(str, REDIR_IN);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, "|", 1) == 0)
		new_token = ft_lstnew_token(str, PIPE);
	return (new_token);
	}

static bool	is_token(char *str)
{
	if (ft_strlen(str) == 2 && ft_strncmp(str, ">>", 2) == 0)
		return (true);
	else if (ft_strlen(str) == 2 && ft_strncmp(str, "<<", 2) == 0)
		return (true);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, ">", 1) == 0)
		return (true);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, "<", 1) == 0)
		return (true);
	else if (ft_strlen(str) == 1 && ft_strncmp(str, "|", 1) == 0)
		return (true);
	else
		return (false);
}

static t_token	*check_tokens(char *str)
{
	t_token	*new_token;

	if (is_token(str))
		new_token = set_token(str);
	else
		new_token = ft_lstnew_token(str, WORD);
	return (new_token);
}

t_token	*ft_tokenizer(char *str)
{
	t_token	*tokens_list;
	t_token	*new_token;
	char	**tokens;
	int		i;

	tokens_list = NULL;
	tokens = ft_split(str, ' ');
	i = 0;
	while (tokens[i])
	{
		new_token = check_tokens(tokens[i]);
		ft_addback_token(&tokens_list, new_token);
		i++;
	}
	ft_free_array(tokens);
	return (tokens_list);
}
