/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:14:18 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/02 00:08:20 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_exit_code(t_hd_exp *hd)
{
	char	*tmp;

	tmp = ft_itoa(hd->exit_code);
	hd->result = ft_strjoin_free(hd->result, tmp);
	free(tmp);
	return (hd->result);
}

static char	*expand_to_value(t_hd_exp *hd, size_t *i, size_t start)
{
	char	*var_name;
	char	*value;

	var_name = ft_substr(hd->line, start, *i - start);
	value = get_value_by_key(hd->env, var_name);
	hd->result = ft_strjoin_free(hd->result, value);
	free(value);
	free(var_name);
	return (hd->result);
}

static void	handle_dollar(t_hd_exp *hd, size_t *i)
{
	size_t	start;

	(*i)++;
	if (hd->line[*i] == '?')
	{
		get_exit_code(hd);
		(*i)++;
	}
	else if (ft_isalpha(hd->line[*i]) || hd->line[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(hd->line[*i]) || hd->line[*i] == '_')
			(*i)++;
		expand_to_value(hd, i, start);
	}
	else
		hd->result = ft_strjoin_char(hd->result, '$');
}

static void	navigate_and_expand(t_hd_exp *hd)
{
	size_t	i;

	i = 0;
	while (hd->line[i])
	{
		if (hd->line[i] == '$')
			handle_dollar(hd, &i);
		else
		{
			hd->result = ft_strjoin_char(hd->result, hd->line[i]);
			i++;
		}
	}
}

char	*expand_heredoc_line(char *line, t_env *env, int exit_status)
{
	t_hd_exp	*hd;
	char		*expanded;

	if (!line)
		return (NULL);
	if (!contains_dollar_heredoc(line))
		return (ft_strdup(line));
	hd = init_hd_expander_data(line, env, exit_status);
	if (!hd)
		return (perror("heredoc expander data init failed"), NULL);
	navigate_and_expand(hd);
	expanded = ft_strdup(hd->result);
	free_hd_expander_data(hd);
	return (expanded);
}
