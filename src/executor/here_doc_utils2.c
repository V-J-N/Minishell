/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:08:30 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/01 12:30:53 by vjan-nie         ###   ########.fr       */
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

char	*expand_heredoc_line(char *line, t_env *env, int exit_status)
{
	size_t	i;
	char	*result;
	char	*tmp;
	size_t	start;
	char	*var_name;
	char	*value;

	if (!line)
		return (NULL);
	if (!contains_dollar_heredoc(line))
		return (ft_strdup(line));
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			if (line[i] == '?')
			{
				tmp = ft_itoa(exit_status);
				result = ft_strjoin_free(result, tmp);
				free(tmp);
				i++;
			}
			else if (ft_isalpha(line[i]) || line[i] == '_')
			{
				start = i;
				while (ft_isalnum(line[i]) || line[i] == '_')
					i++;
				var_name = ft_substr(line, start, i - start);
				value = get_value_by_key(env, var_name);
				result = ft_strjoin_free(result, value);
				free(value);
				free(var_name);
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

