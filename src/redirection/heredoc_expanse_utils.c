/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expanse_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:06:43 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/02 00:09:04 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	contains_dollar_heredoc(char *str)
{
	size_t	i;

	i = 0;
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

t_hd_exp	*init_hd_expander_data(char *line, t_env *env, int exit_status)
{
	t_hd_exp	*hd;

	hd = malloc(sizeof(t_hd_exp));
	if (!hd)
		return (NULL);
	hd->line = line;
	hd->env = env;
	hd->exit_code = exit_status;
	hd->result = ft_strdup("");
	return (hd);
}

void	free_hd_expander_data(t_hd_exp *hd)
{
	if (!hd)
		return ;
	if (hd->result)
		free(hd->result);
	free(hd);
}
