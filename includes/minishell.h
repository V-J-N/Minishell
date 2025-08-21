/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/08/21 18:39:18 by vjan-nie         ###   ########.fr       */
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
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "structs.h"

//ENVIRONMENT:

bool	get_environment(char *envp[], t_env **environment);
void	free_environment(t_env **env);
char	*set_key(char *str);
char	*set_value(char *str);
t_env	*ft_lstnew_mini_env(char *key, char *value, char *full);
void	ft_addback_mini_env(t_env **head, t_env *new_node);
char	*get_value_by_key(t_env *env_list, const char *key);
char	*get_full_env(t_env *env_list, const char *key);

//EXECUTOR_UTILS:
void	cmd_not_found(char *cmd, char **args);
void	ft_free_array(char **array);
char	**ft_potential_paths(char *path_value);
char	*ft_build_full_path(char *command, char *path_value);
char	*ft_check_path(char *command, char *path_value);

//EXECUTOR:
void	execute_command(char *cmd_str, t_env **envp);
void	command_in(char *command, t_env **environment);
char	**envlist_to_arr(t_env **envlist);

#endif