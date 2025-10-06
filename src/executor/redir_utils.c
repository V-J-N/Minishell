/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:32:09 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/06 11:44:30 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_input_redir(t_command *cmd)
{
	t_redir	*temp;

	temp = cmd->redirs;
	while (temp)
	{
		if (temp->type == REDIR_IN || temp->type == HEREDOC)
			return (true);
		temp = temp->next;
	}
	return (false);
}

bool	has_output_redir(t_command *cmd)
{
	t_redir	*temp;

	temp = cmd->redirs;
	while (temp)
	{
		if (temp->type == REDIR_OUT || temp->type == APPEND)
			return (true);
		temp = temp->next;
	}
	return (false);
}

int	get_inputfile_fd(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		return (perror(infile), -1);
	}
	return (fd);
}

int	get_outputfile_fd(char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		return (perror(outfile), -1);
	}
	return (fd);
}

int	get_append_fd(char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (perror(outfile), -1);
	return (fd);
}
