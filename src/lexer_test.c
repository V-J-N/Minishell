/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:15:45 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/08/27 11:06:34 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_token *list)
{
	t_token	*temp;

	temp = list;
	while(temp)
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

	token_list = NULL;
	token_list = ft_tokenizer(argv[1]);
	print_list(token_list);
	free_tokens(&token_list);
	return (0);
} */