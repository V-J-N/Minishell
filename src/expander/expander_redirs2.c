/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redirs2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:25:28 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 22:21:18 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_redirs2.c
 * @brief Detailed logic for expanding environment variables and the
 * exit status within the filenames associated with redirections.
 * This logic is structurally similar to argument expansion, focusing on string
 * manipulation for the filename.
 */
#include "minishell.h"

/**
 * @brief Handles the content found after a '$' sign in a redirection filename
 * and appends the expanded value (or the original character) to the
 * temporary expanded string.
 * It correctly handles positional parameters, the exit status,
 * and standard environment variables ($VAR).
 * @param redir The current redirection node being processed.
 * @param env The head of the environment list.
 * @param exit The status code of the last executed command ($?).
 */
static void	redir_dollar(t_redir *rdr, t_env *env, int exit)
{
	if (rdr->file[rdr->i + 1] == '\0')
	{
		redir_value(rdr, rdr->i);
		return ;
	}
	if (rdr->file[rdr->i] == 36)
	{
		rdr->i++;
		if (ft_isdigit(rdr->file[rdr->i]))
			rdr->i++;
		else if (rdr->file[rdr->i] == 63)
			redir_exit(rdr, exit);
		else if (ft_isalnum(rdr->file[rdr->i]) || rdr->file[rdr->i] == '_')
		{
			rdr->env_file = ft_strdup("");
			if (!rdr->env_file)
				return ;
			while (ft_isalnum(rdr->file[rdr->i]) || rdr->file[rdr->i] == '_')
				redir_environment(rdr, rdr->i);
			if (rdr->env_file)
				get_expand_redir(rdr, env);
		}
	}
	else
		redir_value(rdr, rdr->i);
}

/**
 * @brief Main function to iterate through the list of redirections and perform
 * variable expansion on the filename of each node.
 * This function is the entry point for expanding variables in redirection
 * file paths.
 * @param redir The head of the 't_redir' list for a command.
 * @param env The head of the environment list.
 * @param exit The status code of the last executed command ($?).
 * @return t_redir* Returns the head of the command list, or NULL on
 * memory failure.
 */
t_redir	*expand_redirs(t_redir *redir, t_env *env, int exit)
{
	t_redir	*temp;

	temp = redir;
	while (temp)
	{
		if (temp->is_expanded && contains_dollar(temp->file))
		{
			if (temp->file[temp->i + 1] == '\0')
			{
				temp = temp->next;
				continue ;
			}
			temp->exp_file = ft_strdup("");
			if (!temp->exp_file)
				return (NULL);
			while (temp->file[temp->i])
				redir_dollar(temp, env, exit);
			temp->is_expanded = false;
			if (temp->exp_file)
				change_file(temp);
		}
		temp = temp->next;
	}
	return (temp);
}
