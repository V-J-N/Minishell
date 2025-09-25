/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 13:42:50 by vjan-nie         ###   ########.fr       */
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
# include <sys/stat.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include <limits.h>
# include "structs.h"
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

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
int			  get_inputfile_fd(char *infile);
int			  get_outputfile_fd(char *outfile);
int			  get_append_fd(char *outfile);
int			  get_heredoc_fd(char *limiter);
int			  redirect_in(t_command *command_list, int in_fd);
int			  redirect_out(t_command *command_list, int out_fd);
bool		  prepare_heredocs(t_command *cmd);
bool		  prepare_all_heredocs(t_command *cmd_list);
int			  redirection_only(t_command *cmd, int in, int out);
bool		  has_redirs(t_command *cmd);
bool		  has_input_redir(t_command *cmd);
bool		  has_output_redir(t_command *cmd);

//LEXER
t_token			*tokenizer(char *str);
t_lexer			*init_lexer(char *str);
t_token			*advance_tokenizer(char *str);
t_lexer			*lexer_loop(t_lexer *lexer);
void			free_tokens(t_token **tokens);
void			lstaddback_token(t_token **head, t_token *new_node);
t_token			*lstnew_token(char *value, t_token_type type,
					t_token_quote quote);
char			**tokenlist_to_arr(t_token *tokenlist);
char			*ft_realloc(char *buffer, size_t capacity);
t_token_quote	verify_quotes(char c);
char			*tokenize_buffer(char *buffer, t_token *new_token,
					t_token **list);
t_lexer			*tokenize_char(t_lexer *lexer);
t_lexer			*tokenize_space(t_lexer *lexer);
t_lexer			*tokenize_pipe(t_lexer *lexer);
t_lexer			*tokenize_input(t_lexer *lexer);
t_lexer			*tokenize_output(t_lexer *lexer);

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

//BUILT_INS:
int				built_in(char *cmd, t_env *env, t_command *cmd_lst);
int				execute_builtin(char *cmd, t_env *env, t_command *cmd_lst);

//BI_CHILD:

int				ft_pwd(t_env *env);
int				ft_env(t_env *env);
int				ft_echo(t_command *cmd);

//BI_PARENT:
int				ft_exit(t_command *cmd_lst);
int				ft_unset(t_command *cmd_lst, t_env **env);
int				ft_export(t_command *cmd, t_env **env);
int				ft_cd(t_command *cmd, t_env **env);


//BUILT_IN_UTILS:
bool			is_built_in(char *cmd);
bool			is_parent_built_in(char *cmd);
t_env			*find_node_by_key(t_env *env_list, const char *key);
char			*get_value_by_key(t_env *env_list, const char *key);
char			**args_to_array(t_arg *args);
void			delete_env_key(t_env **env, const char *key);

#endif