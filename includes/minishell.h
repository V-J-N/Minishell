/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:47:33 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/20 13:34:35 by serjimen         ###   ########.fr       */
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
# ifndef SHLVL_MAX
#  define SHLVL_MAX 100
# endif

extern volatile sig_atomic_t	g_sigint_status;
//extern: se usa para declarar una variable global que está definida en otro archivo
//sig_atomic: Es un tipo especial definido por la librería <signal.h>
//Atómico: el sistema puede leer/escribir su valor en una sola operación, sin riesgo de corrupción.
//Ideal para variables compartidas entre el programa normal y los signal handlers.
//Le dice al compilador que el valor de esa variable puede cambiar en cualquier momento,
//fuera del control del programa.
//En este caso, la variable puede cambiar cuando llega una señal, incluso en medio de otra función.
//Sin volatile, el compilador podría optimizar el acceso a la variable
//(por ejemplo, guardarla en un registro), y no notar que cambió su valor. Eso es peligroso cuando trabajas con señales.

//MINISHELL UTILS:
void			sigint_handler(int signum);
void			setup_signals(void);
void			ft_free_array(char **array);
void			ft_cleanup_loop(t_data *data, char *input, bool	error);
void			ft_cleanup_end(t_data *data);

//ENVIRONMENT:
bool			get_environment(char *envp[], t_env **environment);
void			free_environment(t_env **env);
char			*set_key(char *str);
char			*set_value(char *str);
t_env			*ft_lstnew_mini_env(char *key, char *value, char *full);
void			ft_addback_mini_env(t_env **head, t_env *new_node);
char			*get_value_by_key(t_env *env_list, const char *key);
char			*get_full_env(t_env *env_list, const char *key);
bool			shell_lvl_handler(t_data *data);

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
int				execute_all(t_data *data);
int				command_in(t_data *data, int in, int out);
void			execute_command(t_command *command, t_env **envlist);

//PIPES:
int				pipes(t_pipe *pipe_data, int prev_pipe, pid_t last_pid);
void			safe_close(int fd);
void			ft_close_two(int fd1, int fd2);
void			ft_close_three(int fd1, int fd2, int fd3);
t_pipe			*init_pipe_data(t_data *data, size_t commands);
void			free_pipe_data(t_pipe *pipe_data);
void			p_child_process(t_pipe *pipe_data, int prev, int *pipe_fd);

//INPUT_OUTPUT:
int				get_inputfile_fd(char *infile);
int				get_outputfile_fd(char *outfile);
int				get_append_fd(char *outfile);
int				get_heredoc_fd(char *limiter);
int				redirect_in(t_command *command_list, int in_fd);
int				redirect_out(t_command *command_list, int out_fd);
bool			prepare_heredocs(t_command *cmd);
bool			prepare_all_heredocs(t_command *cmd_list);
int				redirection_only(t_command *cmd, int in, int out);
bool			has_redirs(t_command *cmd);
bool			has_input_redir(t_command *cmd);
bool			has_output_redir(t_command *cmd);
int				manage_in_redir(char *file, int *fd, int *last_fd);
int				manage_heredoc(int heredoc_fd, int *fd, int *last_fd);
int				manage_out_redir(t_redir *redir, int *fd, int *last_fd);

//LEXER
t_token			*tokenizer(char *str);
t_lexer			*init_lexer(char *str);
t_token			*advance_tokenizer(char *str);
t_lexer			*lexer_loop(t_lexer *lexer);
t_lexer			*lexer_selector(t_lexer *lexer);
void			free_tokens(t_token **tokens);
void			lstaddback_token(t_token **head, t_token *new_node);
t_token			*lstnew_token(char *value, t_token_type type,
					t_token_quote quote, bool has_quote);
char			**tokenlist_to_arr(t_token *tokenlist);
char			*ft_realloc(char *buffer, size_t capacity);
t_token_quote	verify_quotes(char c);
char			*tokenize_buffer(t_lexer *lexer);
t_lexer			*check_none(t_lexer *lexer);
t_lexer			*check_single(t_lexer *lexer);
t_lexer			*check_double(t_lexer *lexer);
t_lexer			*tokenize_char(t_lexer *lexer);
t_lexer			*tokenize_space(t_lexer *lexer);
t_lexer			*tokenize_pipe(t_lexer *lexer);
t_lexer			*tokenize_input(t_lexer *lexer);
t_lexer			*tokenize_output(t_lexer *lexer);
void			free_lexer(t_lexer *lexer);
t_lexer			*init_lexer_buffer(t_lexer *lexer);
bool			is_char(t_lexer *lexer);
t_token_state	set_state(t_token_quote quote, t_token_state state);
bool			check_quotes(char *str);

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
int				built_in(char *cmd, t_data *data, int exit_return);
int				execute_builtin(char *cmd, t_data *data);

//BI_CHILD:

int				ft_pwd(t_env *env);
int				ft_env(t_env *env, t_command *cmd_lst);
int				ft_echo(t_command *cmd);

//BI_PARENT:
int				ft_exit(t_data *data);
int				ft_unset(t_command *cmd_lst, t_env **env);
int				ft_export(t_command *cmd, t_env **env);
int				ft_assign_in(char *full_var, t_env **env);
void			ft_not_valid(char *var);
void			print_sorted_env(t_env *env);
bool			is_valid_identifier(const char *var);
int				ft_cd(t_command *cmd, t_env **env);

//BUILT_IN_UTILS:
bool			is_built_in(char *cmd);
bool			is_parent_built_in(char *cmd);
t_env			*find_node_by_key(t_env *env_list, const char *key);
char			*get_value_by_key(t_env *env_list, const char *key);
char			**args_to_array(t_arg *args);
void			delete_env_key(t_env **env, const char *key);

//EXPANDER
t_command		*expander(t_command *cmd_list, t_env *env);
t_arg			*check_singles_arg(t_arg *args);
t_arg			*check_doubles_arg(t_arg *args);
t_arg			*change_value(t_arg *args);
t_redir			*check_singles_redir(t_redir *redirs);
t_redir			*check_doubles_redir(t_redir *redirs);
t_redir			*change_file(t_redir *redirs);
bool			contains_dollar(char *str);

#endif