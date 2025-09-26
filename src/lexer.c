/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:33:51 by serjimen          #+#    #+#             */
/*   Updated: 2025/09/26 10:05:02 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* @file lexer.c
* @brief Main function for the lexing phase of minishell.
* This file contains the 'tokenizer' function, which is responsible for 
* converting a raw command-line string into a structured linked list of tokens.
* It is the core of the lexer, coordinating the splitting, classification,
* and list-building processes.
 */
#include "minishell.h"

/**
 * @brief Splits a command string, classifies the resulting substring, and
 * builds a linked list of tokens.
 * The function takes a raw command-line string and performs the following
 * steps:
 * 1. It splits the input string into an array of substring based on spaces.
 * 2. It iterates through this array, classifying each substring using the
 * 'classify_tokens' function to determine its type.
 * 3. It creates a new 't_token' node for each classified token.
 * 4. It appends the new node to the end of a 't_token' linked list.
 * 5. It manages all dynamically allocated memory, ensuring that the temporary
 * array of strings created by 'ft_split' is freed, and that the token list is
 * properly freed in case of an allocation error.
 * @param str The raw command-line string to be tokenized.
 * @return A pointer to the head of the newly created 't_token' linked list.
 * Returns NULL if the input string is invalid or if a memory allocation
 * failure occurs at any point during the process.
 * @note It design with explicit error handling and cleanup, for preventing
 * memory leaks in a long-runnning shell.
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
