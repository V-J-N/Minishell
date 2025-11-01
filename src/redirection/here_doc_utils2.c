/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:08:30 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 21:16:17 by vjan-nie         ###   ########.fr       */
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

static char	*navigate_and_expand(char *line, t_env *env, int exit_status, char *result)
{
	size_t	i;
	size_t	start;
	char	*var_name;
	char	*value;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			if (line[i] == '?')
			{
				result = get_exit_code(result, exit_status);
				i++;
			}
			else if (ft_isalpha(line[i]) || line[i] == '_')
			{
				start = i;
				while (ft_isalnum(line[i]) || line[i] == '_')
					i++;
				result = expand_to_value(line, i, start, env);
			}
			else
				result = ft_strjoin_char(result, '$');
		}
		else
		{
			result = ft_strjoin_char(result, line[i]);
			i++;
		}
	}
	return (result);
}

char	*expand_heredoc_line(char *line, t_env *env, int exit_status)
{
	char	*result;

	if (!line)
		return (NULL);
	if (!contains_dollar_heredoc(line))
		return (ft_strdup(line));
	result = ft_strdup("");
	result = navigate_and_expand(line, env, exit_status, result);
	return (result);
}

