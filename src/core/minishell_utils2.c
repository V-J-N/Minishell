/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/24 01:37:51 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minishell_utils2.c
 * @brief Contains initialization utilities for the main shell data structure.
 */
#include "minishell.h"

/**
 * @brief Initializes the main minishell data structure (t_data).
 * Allocates memory for the t_data structure and initializes its core components
 * to NULL. It then populates the internal environment list from the system's
 * environment array.
 * @param envp The environment array passed to main.
 * @return t_data A pointer to the newly initialized data structure, or NULL
 * on failure.
 */
t_data	*init_data(char **envp)
{
	t_data			*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = NULL;
	data->token = NULL;
	data->parsed = NULL;
	if (!get_environment(envp, &data->env))
	{
		perror("envp copy failed");
		free(data);
		return (NULL);
	}
	return (data);
}
