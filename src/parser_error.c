/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:30:44 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/11 14:03:52 by sergio-jime      ###   ########.fr       */
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
	perror(str);
	free_parser(&parse_struct);
}
