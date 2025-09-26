/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:33:51 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 10:06:34 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file lexer.c
 * @brief Main function for the lexing phase of minishell.
 * This file contains the 'tokenizer' function, which serves as the public
 * entry point for the lexing phase. It is responsible for validating the
 * initial input and delegating the char-by-char analisys to the core
 * lexing logic
 */
#include "minishell.h"

/**
 * @brief Public entry point for converting a command string into a token
 * list.
 * This function acts a wrapper around the core lexing process. Its primary
 * responsibilities are initial input validation and delegating the
 * char-by-char tokenization to the 'advance_tokenizer' utility.
 * @param str The raw command-line string to be tokenized.
 * @return A pointer to the head of the newly created 't_token' linked list.
 * Returns NULL if the input string is invalid or if a memory allocation
 * failure occurs at any point during the process.
 * @note This function maintains a clean interface, focusing solely on receiving
 * the raw input and returning the final structured token list.
 */
t_token	*tokenizer(char *str)
{
	t_token	*tokens_list;

	tokens_list = NULL;
	if (!str || !*str)
		return (NULL);
	tokens_list = advance_tokenizer(str);
	return (tokens_list);
}
