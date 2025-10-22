/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:30:44 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/22 19:45:03 by vjan-nie         ###   ########.fr       */
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
	ft_printf("%s: ", str);
	free_parser(&parse_struct);
}
