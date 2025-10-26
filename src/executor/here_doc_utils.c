/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 08:42:54 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 08:49:10 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint_handler(int sign)
{
	g_exit_code = sign;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	exit(130); 
}
