/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:40:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/11/03 11:42:21 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_canceled(char *true_limiter, int *pipe_fd)
{
	ft_putstr_fd("minishell: warning: here-document didn't reach EOF\n", 2);
	free(true_limiter);
	close(pipe_fd[1]);
	exit(EXIT_FAILURE);
}

bool	is_limiter(char *limiter, char *line)
{
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 \
	&& line[ft_strlen(limiter)] == '\n')
		return (true);
	else
		return (false);
}
