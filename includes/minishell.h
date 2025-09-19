/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio-jimenez <sergio-jimenez@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/18 12:40:46 by sergio-jime      ###   ########.fr       */
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
# include <signal.h>
# include <errno.h>
# include "structs.h"

//MINISHELL UTILS:
void			ft_free_array(char **array);

//ENVIRONMENT:
bool			get_environment(char *envp[], t_env **environment);
void			free_environment(t_env **env);
char			*set_key(char *str);
char			*set_value(char *str);
t_env			*ft_lstnew_mini_env(char *key, char *value, char *full);
void			ft_addback_mini_env(t_env **head, t_env *new_node);
char			*get_value_by_key(t_env *env_list, const char *key);
char			*get_full_env(t_env *env_list, const char *key);

//EXECUTOR_UTILS:
void			cmd_not_found(char *cmd, char **env_arr, char **args);
char			**ft_potential_paths(char **envp);
char			*ft_build_full_path(char *command, char **envp);
char			*ft_check_path(char *command, char **envp);
char			**envlist_to_arr(t_env **envlist);
char			**tokenlist_to_arr(t_token *tokenlist);
char			**command_to_arr(t_command *command);
int				ft_wait_and_exit(pid_t last_pid);

//EXECUTOR:
int				execute_all(t_command *commands, t_env **environment);
int				command_in(t_command *command, t_env **environment,
					int in, int out);
void			execute_command(t_command *command, t_env **envlist);

//PIPES:
int				pipes(t_pipe *pipe_data);
void			safe_close(int fd);
void			ft_close_two(int fd1, int fd2);
void			ft_close_three(int fd1, int fd2, int fd3);
t_pipe			*init_pipe_data(t_command *command, t_env **env_list,
					size_t nbr_of_commands);
void			free_pipe_data(t_pipe *pipe_data);

//INPUT_OUTPUT:
int				get_inputfile_fd(char *infile);
int				get_outputfile_fd(char *outfile);
int				get_append_fd(char *outfile);
int				get_heredoc_fd(char *limiter);
int				redirect_in(t_command *command_list, int in_fd);
int				redirect_out(t_command *command_list, int out_fd);

//LEXER
t_token			*tokenizer(char *str);
t_token			*lstnew_token(char *value, t_token_type type,
					t_token_quote quote);
void			lstaddback_token(t_token **head, t_token *new_node);
void			free_tokens(t_token **tokens);

t_token			*advance_tokenizer(char *str);


//LEXER_TEST:
void			print_list(t_token *list);
void			print_array(char **array);

//PARSER

size_t			number_of_commands(t_command *command_list);//nueva
size_t			number_of_redirs(t_command *command_list);//nueva
t_parse_state	*parse_command(t_token *tokens);
t_command		*create_cmd(t_token *tokens);
t_command		*create_empty_cmd(void);
bool			update_empty_cmd(t_token *tokens, t_command *cmd_node);
bool			add_args(t_token *tokens, t_command *cmd_node);
t_redir			*create_redir(t_token *tokens);
void			add_redir(t_command *cmd_node, t_redir *new_redir);
void			lstaddback_cmd(t_command **head, t_command *new_node);
size_t			lstsize_token_word(t_token *lst);
bool			is_redir(t_token *list);
void			free_commands(t_command **commands);
void			free_parser(t_parse_state **parse_struct);
void			parse_error(const char *str, t_parse_state *parse_struct);
void			free_redirs(t_redir *redirs);
bool			handle_pipe(t_token *tokens, t_parse_state *p_struct);
bool			handle_redir(t_token *tokens, t_parse_state *p_struct);
bool			handle_word(t_token *tokens, t_parse_state *p_struct);

//PARSER TEST
void			print_commands(t_parse_state *commands);

#endif