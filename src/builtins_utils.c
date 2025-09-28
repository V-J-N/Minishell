/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:34:38 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/28 11:56:20 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5) ||
			!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4) ||
			!ft_strncmp(cmd, "env", 4));
}

bool	is_parent_built_in(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "exit", 5));
}

char	**args_to_array(t_arg *args)
{
	size_t	count = 0;
	t_arg	*tmp = args;
	char	**arr;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = ft_calloc(count + 1, sizeof(char *));
	if (!arr)
		return NULL;
	tmp = args;
	for (size_t i = 0; i < count; i++)
	{
		arr[i] = ft_strdup(tmp->value);
		if (!arr[i])
			return (ft_free_array(arr), NULL);
		tmp = tmp->next;
	}
	return arr;
}
