/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/19 23:32:06 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "structs.h"

//ENVP_COPY:
char	**copy_env(char **envp);
int		envp_count(char **envp);
void	free_envp_copy(char **envp);

//PIPEX_UTILS:
void	cmd_not_found(char *cmd, char **args);
void	ft_free_split(char **split);
char	**ft_potential_paths(char **envp);
char	*ft_build_full_path(char *command, char **envp);
char	*ft_check_path(char *command, char **envp);

//PIPEX_EXECUTOR:
void	execute_command(char *cmd_str, char **envp);
void	command_in(char *command, char **envp_copy);

#endif