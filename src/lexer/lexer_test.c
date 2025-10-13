/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:15:45 by sergio-jime       #+#    #+#             */
/*   Updated: 2025/10/11 17:40:32 by serjimen         ###   ########.fr       */
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
		printf("Token Quote [%d]\n", temp->quote);
		printf("Token Expand [%d]\n", temp->is_expanded);
		printf("Token Has Quote [%d]\n", temp->has_quotes);
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
