/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:23:11 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/29 12:31:13 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_print(t_arg *args)
{
	while (args)
	{
		ft_printf("%s", args->value);
		if (args->next)
			ft_printf(" ");
		args = args->next;
	}
	return ;
}

/** @brief  Prints in STDOUT the parameters received.
 * If '-n' flag is given, there is no "\n" at the end.
*/
int	ft_echo(t_command *cmd)
{
	bool	newline;
	t_arg	*temp;
	int		i;

	if (!cmd || ft_strncmp(cmd->args->value, "echo", 5))
		return (EXIT_FAILURE);
	temp = cmd->args->next;
	newline = true;
	while (temp && temp->value && ft_strncmp(temp->value, "-n", 2) == 0)
	{
		i = 2;
		while (temp->value[i] == 'n')
			i++;
		if (temp->value[i] != '\0')
			break ;
		newline = false;
		temp = temp->next;
	}
	echo_print(temp);
	if (newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
