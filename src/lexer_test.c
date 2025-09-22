/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:15:45 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/09/22 11:31:21 by sergio-jime      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_input(int argc, char **argv)
{
	char	*input;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 1;
	k = 0;
	input = ft_calloc(1024, sizeof(char));
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			input[k] = argv[i][j];
			j++;
			k++;
		}
		if ((int)i < (argc - 1))
			input[k++] = ' ';
		i++;

	}
	input[k] = '\0';
	return (input);
}

void	print_list(t_token *list)
{
	t_token	*temp;

	temp = list;
	while (temp)
	{
		printf("Token [%s]\n", temp->value);
		printf("Token Type [%d]\n", temp->type);
		temp = temp->next;
	}
}

void	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("[%d]: %s\n", i, array[i]);
		i ++;
	}
	return ;
}

/* int	main(int argc, char *argv[])
{
	t_token	*token_list;
	char	*input;

	token_list = NULL;
	input = create_input(argc, argv);
	token_list = tokenizer(input);
	print_list(token_list);
	free_tokens(&token_list);
	free(input);
	return (0);
} */
