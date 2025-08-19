/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:08:23 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/17 17:44:55 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp_copy(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i ++;
	}
	free(envp);
}

// Contar variables de envp
int	envp_count(char **envp)
{
	int i = 0;
	while (envp[i])
		i++;
	return (i);
}

// Copia modificable del envp original
char	**copy_env(char **envp)
{
	int		count;
	char	**envp_copy;
	int		i;

	if (!envp)
		return (NULL);
	count = envp_count(envp);
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		envp_copy[i] = ft_strdup(envp[i]);
		i ++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}
