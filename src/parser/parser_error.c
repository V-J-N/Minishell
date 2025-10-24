/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:30:44 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/24 12:15:41 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file parser_error.c
 * @brief Error handling utility for the parser
 */
#include "minishell.h"

/**
 * @brief Displays an error message and frees the parser's state.
 * This function provides a unified error handling mechanism for the parser.
 * When a syntax error, memory allocation failure, or any other fatal parsing
 * issue occurs.
 * @param str A string containing a custom error message.
 * @param parse_struct A pointer to the 't_parse_state' structure.
 */
void	parse_error(const char *str, t_parse_state *parse_struct)
{
	ft_putstr_fd((char *)str, STDERR_FILENO);
	free_parser(&parse_struct);
}
