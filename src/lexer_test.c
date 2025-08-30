/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:15:45 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/08/28 17:13:46 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_list(t_token *list)
{
	t_token	*temp;

	temp = list;
	while (temp)
	{
		printf("%s - ", temp->value);
		printf("%d\n", temp->type);
		temp = temp->next;
	}
}

/* int	main(int argc, char *argv[])
{
	t_token	*token_list;
	(void)argc;
	// (void)argv;

	token_list = NULL;
	token_list = tokenizer(argv[1]);
	print_list(token_list);
	free_tokens(&token_list);
	return (0);
}
 */
