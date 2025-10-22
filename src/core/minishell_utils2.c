/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/22 19:30:26 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
