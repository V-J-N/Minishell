/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_heredoc_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:14:18 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 21:14:35 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	contains_dollar_heredoc(char *str)
{
	size_t	i = 0;

	while (str && str[i])
	{
		if (str[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (ft_strjoin_free(s, tmp));
}

static char	*get_exit_code(char *result, int exit_status)
{
	char	*tmp;

	tmp = ft_itoa(exit_status);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	return (result);
}

static char *expand_to_value(char *line, size_t i, size_t start, t_env *env)
{
	char	*var_name;
	char	*value;
	char	*result;

	var_name = ft_substr(line, start, i - start);
	value = get_value_by_key(env, var_name);
	result = ft_strjoin_free(result, value);
	free(value);
	free(var_name);
	return (result);
}

static char	*navigate_and_expand(t_hd_exp **hd_data, char *var_name, char *value, size_t i)
{
	size_t	start;

	while (hd_data->line[i])
	{
		if (hd_data->line[i] == '$')
		{
			i++;
			if (hd_data->line[i] == '?')
			{
				hd_data->result = get_exit_code(hd_data->result, hd_data->exit_code);
				i++;
			}
			else if (ft_isalpha(hd_data->line[i]) || hd_data->line[i] == '_')
			{
				start = i;
				while (ft_isalnum(hd_data->line[i]) || hd_data->line[i] == '_')
					i++;
				hd_data->result = expand_to_value(hd_data->line, i, start, hd_data->env);
			}
			else
				hd_data->result = ft_strjoin_char(hd_data->result, '$');
		}
		else
		{
			hd_data->result = ft_strjoin_char(hd_data->result, hd_data->line[i]);
			i++;
		}
	}
}

static char	*prepare_expand(t_hd_exp **hd_data)
{
	size_t	i;
	size_t	start;
	char	*var_name;
	char	*value;

	i = 0;
	while (hd_data->line[i])
	{
		if (hd_data->line[i] == '$')
		{
			i++;
			if (hd_data->line[i] == '?')
			{
				hd_data->result = get_exit_code(hd_data->result, hd_data->exit_code);
				i++;
			}
			else if (ft_isalpha(hd_data->line[i]) || hd_data->line[i] == '_')
			{
				start = i;
				while (ft_isalnum(hd_data->line[i]) || hd_data->line[i] == '_')
					i++;
				hd_data->result = expand_to_value(hd_data->line, i, start, hd_data->env);
			}
			else
				hd_data->result = ft_strjoin_char(hd_data->result, '$');
		}
		else
		{
			hd_data->result = ft_strjoin_char(hd_data->result, hd_data->line[i]);
			i++;
		}
	}
	return (hd_data->result);
}

static void	init_hd_expander_data(t_hd_exp **hd_data, char *line, t_env *env, int exit_status)
{
	hd_data->env = env;
	hd_data->exit_code = exit_status;
	hd_data->line = line;
	hd_data->result = ft_strdup("");
	return ;
}

char	*expand_heredoc_line(char *line, t_env *env, int exit_status)
{
	char		*expanded;
	t_hd_exp	*hd_data;

	if (!line)
		return (NULL);
	init_hd_expander_data(hd_data, line, env, exit_status);
	if (!contains_dollar_heredoc(line))
		return (ft_strdup(line));
	expanded = prepare_expand(hd_data);
	return (expanded);
}
