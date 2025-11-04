/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:26:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/11/03 22:59:04 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expander_redirs.c
 * @brief Handles the final stage of redirection filename processing:
 * calculating the final string length and removing the surrounding quotes.
 */
#include "minishell.h"

/**
 * @brief Copies the redirection filename (`file`) into the expanded filename
 * (`exp_file`), removing any double quotes (ASCII 34).
 * @param redirs The redirection node (t_redir) to process.
 */
static void	copy_file_double(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!redirs || !redirs->file)
		return ;
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
 * @brief Copies the redirection filename (`file`) into the expanded filename
 * (`exp_file`), removing any single quotes (ASCII 39).
 * @param redirs The redirection node (t_redir) to process.
 */
static void	copy_file_single(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!redirs || !redirs->file)
		return ;
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

/**
 * @brief Finalizes the filename value change. It frees the old filename,
 * duplicates the final processed string from `exp_file` into `file`,
 * and frees `exp_file`.
 * This function consolidates the result of expansion and quote stripping for
 * the filename.
 * @param redirs The redirection node (t_redir) containing the final string
 * in `exp_file`.
 * @return t_redir* Returns the same redirection node (`temp`), or NULL if input
 * is invalid.
 */
t_redir	*change_file(t_redir *redirs)
{
	t_redir	*temp;

	if (!redirs || !redirs->file)
		return (NULL);
	temp = redirs;
	free(temp->file);
	temp->file = ft_strdup(temp->exp_file);
	free(temp->exp_file);
	temp->exp_file = NULL;
	return (temp);
}

/**
 * @brief Calculates the length of the filename after removing double quotes,
 * allocates memory for the new string (`exp_file`), and performs the copy.
 * This is the primary function for stripping double quotes from a redirection
 * filename.
 * @param redirs The redirection node to check.
 * @return t_redir* The processed redirection node, or NULL on failure.
 */
t_redir	*check_doubles_redir(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	if (!redirs || !redirs->file)
		return (NULL);
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
 * @brief Calculates the length of the filename after removing single quotes,
 * allocates memory for the new string (`exp_file`), and performs the copy.
 * This is the primary function for stripping single quotes from a redirection
 * filename.
 * @param redirs The redirection node to check.
 * @return t_redir* The processed redirection node, or NULL on failure.
 */
t_redir	*check_singles_redir(t_redir *redirs)
{
	t_redir	*temp;
	size_t	i;
	size_t	quotes;
	size_t	len;

	i = 0;
	quotes = 0;
	if (!redirs || !redirs->file)
		return (NULL);
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
