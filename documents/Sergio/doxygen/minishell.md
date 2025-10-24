```c
/**
 * @file minishell.h
 * @brief Main header file for the minishell project.
 *
 * This file includes all necessary system libraries, external project headers,
 * macro definitions, and function prototypes for the entire minishell application.
 */
#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/* Project Headers */

# include "libft.h"
# include "structs.h"

/* ************************************************************************** */
/* System Headers */

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <signal.h>
# include <errno.h>
# include <limits.h>

/* ************************************************************************** */
/* Readline Headers */

# include <readline/readline.h>
# include <readline/history.h>

/* ************************************************************************** */
/* Signal handling */

/**
 * @brief Global variable to track SIGINT status (Ctrl+C).
 *
 * This volatile variable is safely used across the main loop and signal handlers
 * to track interrupt events, typically setting the exit status.
 */
extern volatile sig_atomic_t	g_sigint_status;

/* ************************************************************************** */
/* Macros */

/** @brief The maximum length for a file path (if not defined by the system). */
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif
/** @brief The maximum shell level (SHLVL) supported before a warning is issued. */
# ifndef SHLVL_MAX
#  define SHLVL_MAX 100
# endif

/* ************************************************************************** */
/* Minishell Core and Utilities */

/**
 * @brief Initializes the main data structure for the shell.
 * @param envp The environment array passed from the main function.
 * @return t_data* A pointer to the initialized data structure.
 */
t_data	 		*init_data(char **envp);

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * @param signum The signal number received (expected to be SIGINT).
 */
void	 		sigint_handler(int signum);

/**
 * @brief Sets up signal handlers for the shell's main process.
 */
void	 		setup_signals(void);

/**
 * @brief Frees a dynamically allocated NULL-terminated array of strings.
 * @param array The array to be freed.
 */
void	 		ft_free_array(char **array);

/**
 * @brief Performs cleanup routines after each command loop.
 * @param data The main data structure.
 * @param input The command line input string.
 * @param error Flag indicating if an error occurred during the loop.
 */
void	 		ft_cleanup_loop(t_data *data, char *input, bool	error);

/**
 * @brief Performs final cleanup before the shell exits.
 * @param data The main data structure.
 */
void	 		ft_cleanup_end(t_data *data);

/* ************************************************************************** */
/* Environment Management */

/**
 * @brief Initializes the environment linked list from the system envp array.
 * @param envp The system environment array.
 * @param environment Pointer to the head of the t_env list to be populated.
 * @return bool True on success, false on memory allocation failure.
 */
bool	 		get_environment(char *envp[], t_env **environment);

/**
 * @brief Deallocates all nodes and their contents in the environment linked list.
 * @param env A double pointer to the head of the t_env list.
 */
void	 		free_environment(t_env **env);

/**
 * @brief Extracts and allocates a deep copy of the key from an environment string.
 * @param str The source "KEY=value" string.
 * @return char* A new dynamically allocated string containing the key.
 */
char	 		*set_key(char *str);

/**
 * @brief Extracts and allocates a deep copy of the value from an environment string.
 * @param str The source "KEY=value" string.
 * @return char* A new dynamically allocated string containing the value.
 */
char	 		*set_value(char *str);

/**
 * @brief Creates and initializes a new environment node (constructor).
 * @param key The environment key string.
 * @param value The environment value string.
 * @param full The full "KEY=value" string.
 * @return t_env* A pointer to the newly created node.
 */
t_env	 		*ft_lstnew_mini_env(char *key, char *value, char *full);

/**
 * @brief Adds a new environment node to the end of the linked list.
 * @param head A pointer to pointer to the head of the t_env list.
 * @param new_node A pointer to the new t_env node.
 */
void	 		ft_addback_mini_env(t_env **head, t_env *new_node);

/**
 * @brief Searches the environment list for a key and returns a deep copy of its value.
 * @param env_list The head of the t_env list.
 * @param key The environment variable name to search for.
 * @return char* A new allocated string containing the value, or NULL if not found.
 */
char	 		*get_value_by_key(t_env *env_list, const char *key);

/**
 * @brief Searches the environment list for a key and returns a deep copy of its full string ("KEY=value").
 * @param env_list The head of the t_env list.
 * @param key The environment variable name to search for.
 * @return char* A new allocated string containing the full environment variable, or NULL if not found.
 */
char	 		*get_full_env(t_env *env_list, const char *key);

/**
 * @brief Handles the SHLVL environment variable increment and potential warnings.
 * @param data The main data structure containing the environment list.
 * @return bool True on success, false on error.
 */
bool	 		shell_lvl_handler(t_data *data);

/* ************************************************************************** */
/* Executor Utilities */

/**
 * @brief Prints an error message for a command not found, and cleans up memory.
 * @param cmd The command name that was not found.
 * @param env_arr The environment array (to be freed).
 * @param args The command arguments array (to be freed).
 */
void	 		cmd_not_found(char *cmd, char **env_arr, char **args);

/**
 * @brief Extracts and splits the PATH variable from the environment into an array.
 * @param envp The environment array.
 * @return char** An array of potential path directories, or NULL if PATH is not found.
 */
char	 		**ft_potential_paths(char **envp);

/**
 * @brief Tries to construct the full executable path by checking PATH directories.
 * @param command The command name.
 * @param envp The environment array.
 * @return char* The fully qualified path to the command, or NULL if not found.
 */
char	 		*ft_build_full_path(char *command, char **envp);

/**
 * @brief Finds the executable path for a command, handling absolute/relative paths and PATH lookup.
 * @param command The command name.
 * @param envp The environment array.
 * @return char* The final path to the executable, or NULL on failure.
 */
char	 		*ft_check_path(char *command, char **envp);

/**
 * @brief Converts the environment linked list (t_env) to a NULL-terminated char** array (for execve).
 * @param envlist A pointer to the head of the t_env list.
 * @return char** A new array of "KEY=value" strings.
 */
char	 		**envlist_to_arr(t_env **envlist);

/**
 * @brief Converts the token list (t_token) to a NULL-terminated char** array.
 * @param tokenlist The head of the t_token list.
 * @return char** A new array of token values.
 */
char	 		**tokenlist_to_arr(t_token *tokenlist);

/**
 * @brief Converts the arguments of a single command (t_command) into a NULL-terminated char** array (for execve).
 * @param command The t_command node.
 * @return char** A new array of command arguments.
 */
char	 		**command_to_arr(t_command *command);

/**
 * @brief Waits for the last child process to finish and sets the shell's exit status.
 * @param last_pid The PID of the last command in the pipeline.
 * @return int The exit status of the last command.
 */
int		 		ft_wait_and_exit(pid_t last_pid);

/* ************************************************************************** */
/* Executor */

/**
 * @brief Orchestrates the execution of the entire command pipeline.
 * @param data The main data structure containing the command list.
 * @return int The exit status of the last executed command.
 */
int		 		execute_all(t_data *data);

/**
 * @brief Manages a single command execution within the pipeline (handles redirection).
 * @param data The main data structure.
 * @param in The input file descriptor for the command.
 * @param out The output file descriptor for the command.
 * @return int The exit status of the command.
 */
int		 		command_in(t_data *data, int in, int out);

/**
 * @brief Executes a single external command using fork and execve.
 * @param command The t_command node to execute.
 * @param envlist A pointer to the environment list.
 */
void	 		execute_command(t_command *command, t_env **envlist);

/* ************************************************************************** */
/* Pipes and Process Management */

/**
 * @brief Main recursive function for piping and forking processes.
 * @param pipe_data The t_pipe structure containing execution context.
 * @param prev_pipe The file descriptor of the input pipe from the previous command.
 * @param last_pid The PID of the previously spawned process.
 * @return int The PID of the last process spawned.
 */
int		 		pipes(t_pipe *pipe_data, int prev_pipe, pid_t last_pid);

/**
 * @brief Safely closes a file descriptor if it is valid (not -1).
 * @param fd The file descriptor to close.
 */
void	 		safe_close(int fd);

/**
 * @brief Closes two file descriptors safely.
 * @param fd1 First file descriptor.
 * @param fd2 Second file descriptor.
 */
void	 		ft_close_two(int fd1, int fd2);

/**
 * @brief Closes three file descriptors safely.
 * @param fd1 First file descriptor.
 * @param fd2 Second file descriptor.
 * @param fd3 Third file descriptor.
 */
void	 		ft_close_three(int fd1, int fd2, int fd3);

/**
 * @brief Initializes the t_pipe structure for execution context.
 * @param data The main data structure.
 * @param commands The total number of commands in the pipeline.
 * @return t_pipe* A pointer to the initialized t_pipe structure.
 */
t_pipe	 		*init_pipe_data(t_data *data, size_t commands);

/**
 * @brief Frees the t_pipe structure and closes its associated file descriptors.
 * @param pipe_data The t_pipe structure to be freed.
 */
void	 		free_pipe_data(t_pipe *pipe_data);

/**
 * @brief Logic executed by the child process in a pipe.
 * @param pipe_data The t_pipe structure.
 * @param prev The input file descriptor from the previous pipe.
 * @param pipe_fd The file descriptors for the current pipe output (write end).
 */
void	 		p_child_process(t_pipe *pipe_data, int prev, int *pipe_fd);

/* ************************************************************************** */
/* Input/Output and Redirections */

/**
 * @brief Opens a file for input redirection ('<').
 * @param infile The path to the input file.
 * @return int The file descriptor, or -1 on error.
 */
int		 		get_inputfile_fd(char *infile);

/**
 * @brief Opens a file for output redirection ('>').
 * @param outfile The path to the output file.
 * @return int The file descriptor, or -1 on error.
 */
int		 		get_outputfile_fd(char *outfile);

/**
 * @brief Opens a file for append redirection ('>>').
 * @param outfile The path to the output file.
 * @return int The file descriptor, or -1 on error.
 */
int		 		get_append_fd(char *outfile);

/**
 * @brief Creates a temporary file and reads lines until the delimiter is reached (HEREDOC).
 * @param limiter The delimiter string.
 * @return int The file descriptor of the temporary heredoc file, or -1 on error.
 */
int		 		get_heredoc_fd(char *limiter);

/**
 * @brief Applies all input redirections for a command, returning the final input FD.
 * @param command_list The t_command node.
 * @param in_fd The initial input file descriptor.
 * @return int The final effective input file descriptor.
 */
int		 		redirect_in(t_command *command_list, int in_fd);

/**
 * @brief Applies all output redirections for a command, returning the final output FD.
 * @param command_list The t_command node.
 * @param out_fd The initial output file descriptor.
 * @return int The final effective output file descriptor.
 */
int		 		redirect_out(t_command *command_list, int out_fd);

/**
 * @brief Prepares and executes heredoc input reading for a single command.
 * @param cmd The t_command node.
 * @return bool True on success, false on error/interrupt.
 */
bool	 		prepare_heredocs(t_command *cmd);

/**
 * @brief Iterates through the entire command list to prepare all heredocs sequentially.
 * @param cmd_list The head of the t_command list.
 * @return bool True on success, false on error/interrupt.
 */
bool	 		prepare_all_heredocs(t_command *cmd_list);

/**
 * @brief Executes a command that consists only of redirections (no executable).
 * @param cmd The t_command node.
 * @param in The input FD.
 * @param out The output FD.
 * @return int The exit status (0 or 1 on redirection error).
 */
int		 		redirection_only(t_command *cmd, int in, int out);

/**
 * @brief Checks if a command node has any redirections attached.
 * @param cmd The t_command node.
 * @return bool True if redirs is not NULL, false otherwise.
 */
bool	 		has_redirs(t_command *cmd);

/**
 * @brief Checks if a command node has any input redirections ('<' or '<<').
 * @param cmd The t_command node.
 * @return bool True if an input redirection is found.
 */
bool	 		has_input_redir(t_command *cmd);

/**
 * @brief Checks if a command node has any output redirections ('>' or '>>').
 * @param cmd The t_command node.
 * @return bool True if an output redirection is found.
 */
bool	 		has_output_redir(t_command *cmd);

/**
 * @brief Utility to manage input file redirection logic during execution.
 * @param file The filename.
 * @param fd Current effective FD pointer.
 * @param last_fd Last FD pointer to close previous FDs.
 * @return int Success status.
 */
int		 		manage_in_redir(char *file, int *fd, int *last_fd);

/**
 * @brief Utility to manage heredoc file descriptor logic during execution.
 * @param heredoc_fd The file descriptor from the prepared heredoc.
 * @param fd Current effective FD pointer.
 * @param last_fd Last FD pointer to close previous FDs.
 * @return int Success status.
 */
int		 		manage_heredoc(int heredoc_fd, int *fd, int *last_fd);

/**
 * @brief Utility to manage output file redirection logic during execution.
 * @param redir The t_redir node.
 * @param fd Current effective FD pointer.
 * @param last_fd Last FD pointer to close previous FDs.
 * @return int Success status.
 */
int		 		manage_out_redir(t_redir *redir, int *fd, int *last_fd);

/* ************************************************************************** */
/* Lexer (Tokenizer) */

/**
 * @brief Main entry point for the lexer, converts input string to token list.
 * @param str The command line input string.
 * @return t_token* The head of the resulting token linked list.
 */
t_token	 		*tokenizer(char *str);

/**
 * @brief Allocates and initializes the t_lexer state structure.
 * @param str The input string.
 * @return t_lexer* A pointer to the initialized state structure.
 */
t_lexer	 		*init_lexer(char *str);

/**
 * @brief Advances the tokenizer state by one character.
 * @param str The input string.
 * @return t_token* The final token list.
 */
t_token	 		*advance_tokenizer(char *str);

/**
 * @brief The main loop for the lexer, consuming the input string.
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*lexer_loop(t_lexer *lexer);

/**
 * @brief Selects the next tokenizing step based on the current state (quoted/unquoted).
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*lexer_selector(t_lexer *lexer);

/**
 * @brief Frees the entire token linked list.
 * @param tokens A pointer to the head of the t_token list.
 */
void	 		free_tokens(t_token **tokens);

/**
 * @brief Adds a new token node to the end of the list.
 * @param head A pointer to pointer to the head of the t_token list.
 * @param new_node The new token node.
 */
void	 		lstaddback_token(t_token **head, t_token *new_node);

/**
 * @brief Creates and initializes a new token node (constructor).
 * @param value The token's string value.
 * @param type The token's classified type.
 * @param quote The quoting context.
 * @param has_quote Flag indicating if quotes were present.
 * @return t_token* A pointer to the new token node.
 */
t_token	 		*lstnew_token(char *value, t_token_type type,
	t_token_quote quote, bool has_quote);

/**
 * @brief Converts the token list to a NULL-terminated array of strings.
 * @param tokenlist The head of the t_token list.
 * @return char** A new array of token values.
 */
char	 		**tokenlist_to_arr(t_token *tokenlist);

/**
 * @brief Dynamically resizes the character buffer used by the lexer.
 * @param buffer The current buffer.
 * @param capacity The new required capacity.
 * @return char* The reallocated buffer.
 */
char	 		*ft_realloc(char *buffer, size_t capacity);

/**
 * @brief Classifies a character as a single or double quote.
 * @param c The character to check.
 * @return t_token_quote The quote type (SINGLE, DOUBLE, or NONE).
 */
t_token_quote	verify_quotes(char c);

/**
 * @brief Finalizes the current buffer content into a new token.
 * @param lexer The t_lexer state structure.
 * @return char* The processed token value.
 */
char	 		*tokenize_buffer(t_lexer *lexer);

/**
 * @brief Handles tokenization when the lexer is in the unquoted state (OUT).
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*check_none(t_lexer *lexer);

/**
 * @brief Handles tokenization when the lexer is inside single quotes (IN_SINGLE).
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*check_single(t_lexer *lexer);

/**
 * @brief Handles tokenization when the lexer is inside double quotes (IN_DOUBLE).
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*check_double(t_lexer *lexer);

/**
 * @brief Handles character accumulation into the buffer.
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*tokenize_char(t_lexer *lexer);

/**
 * @brief Handles whitespace characters, often finalizing the current word token.
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*tokenize_space(t_lexer *lexer);

/**
 * @brief Handles the PIPE operator ('|').
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*tokenize_pipe(t_lexer *lexer);

/**
 * @brief Handles input redirection operators ('<' and '<<').
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*tokenize_input(t_lexer *lexer);

/**
 * @brief Handles output redirection operators ('>' and '>>').
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*tokenize_output(t_lexer *lexer);

/**
 * @brief Frees the t_lexer state structure and its internal buffer.
 * @param lexer The t_lexer state structure.
 */
void	 		free_lexer(t_lexer *lexer);

/**
 * @brief Initializes or resets the lexer's internal character buffer.
 * @param lexer The t_lexer state structure.
 * @return t_lexer* The updated state structure.
 */
t_lexer	 		*init_lexer_buffer(t_lexer *lexer);

/**
 * @brief Checks if the current character is a regular character (not whitespace or operator).
 * @param lexer The t_lexer state structure.
 * @return bool True if it's a regular character.
 */
bool	 		is_char(t_lexer *lexer);

/**
 * @brief Sets the new quoting state based on the quote character encountered.
 * @param quote The quote character type.
 * @param state The current lexer state.
 * @return t_token_state The new lexer state.
 */
t_token_state	set_state(t_token_quote quote, t_token_state state);

/**
 * @brief Performs a preliminary check for unclosed quotes in the input string.
 * @param str The command line input string.
 * @return bool True if quotes are balanced, false otherwise (syntax error).
 */
bool	 		check_quotes(char *str);

/* ************************************************************************** */
/* Parser */

/**
 * @brief Counts the total number of command blocks in the pipeline.
 * @param command_list The head of the t_command list.
 * @return size_t The number of commands.
 */
size_t	 		number_of_commands(t_command *command_list);

/**
 * @brief Counts the number of redirections attached to a single command block.
 * @param command_list The t_command node.
 * @return size_t The number of redirections.
 */
size_t	 		number_of_redirs(t_command *command_list);

/**
 * @brief Main entry point for the parser, converts token list to command list.
 * @param tokens A pointer to the head of the t_token list.
 * @return t_parse_state* The state structure containing the final command list.
 */
t_parse_state	*parse_command(t_token **tokens);

/**
 * @brief Creates a new t_command node, initializing it with the command name from the token.
 * @param tokens The WORD token containing the command name.
 * @return t_command* A new, initialized command node.
 */
t_command	 	*create_cmd(t_token *tokens);

/**
 * @brief Creates a t_command node initialized to handle redirections that precede the command name.
 * @return t_command* A new, empty command node.
 */
t_command	 	*create_empty_cmd(void);

/**
 * @brief Updates a previously created empty command node with the actual command name.
 * @param tokens The WORD token containing the command name.
 * @param cmd_node The empty t_command node to update.
 * @return bool True on success, false on memory error.
 */
bool	 		update_empty_cmd(t_token *tokens, t_command *cmd_node);

/**
 * @brief Adds a token as an argument to the current command node's argument list.
 * @param tokens The WORD token to add as an argument.
 * @param cmd_node The t_command node.
 * @return bool True on success, false on memory error.
 */
bool	 		add_args(t_token *tokens, t_command *cmd_node);

/**
 * @brief Creates a new t_redir node from the redirection operator and file tokens.
 * @param tokens The redirection operator token.
 * @return t_redir* A new, initialized redirection node.
 */
t_redir	 		*create_redir(t_token *tokens);

/**
 * @brief Adds a redirection node to the current command node's redirection list.
 * @param cmd_node The t_command node.
 * @param new_redir The t_redir node to add.
 */
void	 		add_redir(t_command *cmd_node, t_redir *new_redir);

/**
 * @brief Appends a command node to the end of the main pipeline list.
 * @param head A pointer to pointer to the head of the t_command list.
 * @param new_node The t_command node to append.
 */
void	 		lstaddback_cmd(t_command **head, t_command *new_node);

/**
 * @brief Counts the number of consecutive WORD tokens starting from a given node.
 * @param lst The starting t_token node.
 * @return size_t The count of consecutive WORD tokens.
 */
size_t	 		lstsize_token_word(t_token *lst);

/**
 * @brief Checks if a token represents a redirection operator.
 * @param list The t_token node.
 * @return bool True if the token is a redirection type.
 */
bool	 		is_redir(t_token *list);

/**
 * @brief Frees the entire command pipeline (t_command list) and all its internal structures.
 * @param commands A pointer to the head of the t_command list.
 */
void	 		free_commands(t_command **commands);

/**
 * @brief Frees the t_parse_state structure and the command list it contains.
 * @param parse_struct A pointer to the t_parse_state structure.
 */
void	 		free_parser(t_parse_state **parse_struct);

/**
 * @brief Prints a parser-specific syntax error message and cleans up the parser state.
 * @param str The error message.
 * @param parse_struct The t_parse_state structure to be cleaned up.
 */
void	 		parse_error(const char *str, t_parse_state *parse_struct);

/**
 * @brief Frees the redirection linked list attached to a command node.
 * @param redirs The head of the t_redir list.
 */
void	 		free_redirs(t_redir *redirs);

/**
 * @brief Handler for the PIPE token, finalizing the current command block.
 * @param tokens The PIPE token.
 * @param p_struct The t_parse_state.
 * @return bool True on successful processing and valid syntax.
 */
bool	 		handle_pipe(t_token *tokens, t_parse_state *p_struct);

/**
 * @brief Handler for redirection tokens, creating and attaching a t_redir node.
 * @param tokens The redirection operator token.
 * @param p_struct The t_parse_state.
 * @return bool True on successful processing and valid syntax.
 */
bool	 		handle_redir(t_token *tokens, t_parse_state *p_struct);

/**
 * @brief Handler for WORD tokens, incorporating them as command name or arguments.
 * @param tokens The WORD token.
 * @param p_struct The t_parse_state.
 * @return bool True on success.
 */
bool	 		handle_word(t_token *tokens, t_parse_state *p_struct);

/* ************************************************************************** */
/* Builtin Execution (Central Dispatcher) */

/**
 * @brief Central dispatcher to check if a command is a builtin.
 * @param cmd The command name.
 * @param data The main data structure.
 * @param exit_return The exit status to use if the builtin is 'exit'.
 * @return int The exit status of the executed builtin, or -1 if not a builtin.
 */
int		 		built_in(char *cmd, t_data *data, int exit_return);

/**
 * @brief Executes a recognized builtin command.
 * @param cmd The command name.
 * @param data The main data structure.
 * @return int The exit status of the executed builtin.
 */
int		 		execute_builtin(char *cmd, t_data *data);

/* ************************************************************************** */
/* Builtin Commands (Child Process Compatible) */

/**
 * @brief Implements the 'pwd' builtin: prints the current working directory.
 * @param env The environment list (used for path/error context).
 * @return int The exit status (0 on success).
 */
int		 		ft_pwd(t_env *env);

/**
 * @brief Implements the 'env' builtin: prints the environment variables.
 * @param env The environment list.
 * @param cmd_lst The command list node (for argument check).
 * @return int The exit status (0 on success, 1 on error).
 */
int		 		ft_env(t_env *env, t_command *cmd_lst);

/**
 * @brief Implements the 'echo' builtin: prints arguments.
 * @param cmd The command node containing arguments.
 * @return int The exit status (0 on success).
 */
int		 		ft_echo(t_command *cmd);

/* ************************************************************************** */
/* Builtin Commands (Parent Process Only) */

/**
 * @brief Implements the 'exit' builtin: terminates the shell.
 * @param data The main data structure.
 * @return int The status code passed to the exit function.
 */
int		 		ft_exit(t_data *data);

/**
 * @brief Implements the 'unset' builtin: removes environment variables.
 * @param cmd_lst The command node containing variables to unset.
 * @param env A pointer to the environment list head.
 * @return int The exit status (0 on success, 1 on error).
 */
int		 		ft_unset(t_command *cmd_lst, t_env **env);

/**
 * @brief Implements the 'export' builtin: sets or displays environment variables.
 * @param cmd The command node containing variables to export.
 * @param env A pointer to the environment list head.
 * @return int The exit status (0 on success, 1 on error).
 */
int		 		ft_export(t_command *cmd, t_env **env);

/**
 * @brief Assigns or updates a variable in the environment list.
 * @param full_var The "KEY=value" string.
 * @param env A pointer to the environment list head.
 * @return int The exit status (0 on success, 1 on error).
 */
int		 		ft_assign_in(char *full_var, t_env **env);

/**
 * @brief Prints an error message for an invalid identifier in export/unset.
 * @param var The invalid variable name.
 */
void	 		ft_not_valid(char *var);

/**
 * @brief Prints the environment list in a sorted "declare -x" format (for export with no args).
 * @param env The environment list.
 */
void	 		print_sorted_env(t_env *env);

/**
 * @brief Checks if a string is a valid shell variable identifier.
 * @param var The variable string.
 * @return bool True if valid, false otherwise.
 */
bool	 		is_valid_identifier(const char *var);

/**
 * @brief Implements the 'cd' builtin: changes the current directory.
 * @param cmd The command node containing the target directory.
 * @param env A pointer to the environment list head.
 * @return int The exit status (0 on success, 1 on error).
 */
int		 		ft_cd(t_command *cmd, t_env **env);

/* ************************************************************************** */
/* Builtin Utilities */

/**
 * @brief Checks if a command is one of the builtins.
 * @param cmd The command name.
 * @return bool True if it's a builtin.
 */
bool	 		is_built_in(char *cmd);

/**
 * @brief Checks if a command is one of the builtins that MUST run in the parent process (cd, export, unset, exit).
 * @param cmd The command name.
 * @return bool True if it's a parent-only builtin.
 */
bool	 		is_parent_built_in(char *cmd);

/**
 * @brief Finds an environment node by its key.
 * @param env_list The head of the t_env list.
 * @param key The variable key.
 * @return t_env* The matching node, or NULL.
 */
t_env	 		*find_node_by_key(t_env *env_list, const char *key);

/**
 * @brief Gets the value of an environment variable by key (returns a deep copy).
 * @param env_list The head of the t_env list.
 * @param key The variable key.
 * @return char* A new allocated string with the value, or NULL.
 */
char	 		*get_value_by_key(t_env *env_list, const char *key);

/**
 * @brief Converts the argument list of a command (t_arg) into a NULL-terminated char** array.
 * @param args The head of the t_arg list.
 * @return char** A new array of argument strings.
 */
char	 		**args_to_array(t_arg *args);

/**
 * @brief Deletes an environment variable node from the list by key.
 * @param env A pointer to the environment list head.
 * @param key The key of the variable to delete.
 */
void	 		delete_env_key(t_env **env, const char *key);

/* ************************************************************************** */
/* Expander (Variable and Quote Handling) */

/**
 * @brief Main entry point for the expansion phase.
 * @param cmd_list The parsed command list.
 * @param env The environment list.
 * @param exit_signal The shell's last exit status ($?).
 * @return t_command* The modified command list.
 */
t_command	 	*expander(t_command *cmd_list, t_env *env, int exit_signal);

/**
 * @brief Removes single quotes from arguments.
 * @param args The head of the t_arg list.
 * @return t_arg* The modified argument list.
 */
t_arg	 		*check_singles_arg(t_arg *args);

/**
 * @brief Removes double quotes from arguments.
 * @param args The head of the t_arg list.
 * @return t_arg* The modified argument list.
 */
t_arg	 		*check_doubles_arg(t_arg *args);

/**
 * @brief Performs the final value replacement and quote removal for arguments.
 * @param args The t_arg node to modify.
 * @return t_arg* The modified argument node.
 */
t_arg	 		*change_value(t_arg *args);

/**
 * @brief Removes single quotes from redirection filenames/delimiters.
 * @param redirs The head of the t_redir list.
 * @return t_redir* The modified redirection list.
 */
t_redir	 		*check_singles_redir(t_redir *redirs);

/**
 * @brief Removes double quotes from redirection filenames/delimiters.
 * @param redirs The head of the t_redir list.
 * @return t_redir* The modified redirection list.
 */
t_redir	 		*check_doubles_redir(t_redir *redirs);

/**
 * @brief Performs the final value replacement and quote removal for redirection file names.
 * @param redirs The t_redir node to modify.
 * @return t_redir* The modified redirection node.
 */
t_redir	 		*change_file(t_redir *redirs);

/**
 * @brief Checks if a string contains the '$' character for potential expansion.
 * @param str The string to check.
 * @return bool True if '$' is found, false otherwise.
 */
bool	 		contains_dollar(char *str);

/**
 * @brief Expands environment variables and exit status in command arguments.
 * @param args The head of the t_arg list.
 * @param env The environment list.
 * @param exit The shell's last exit status.
 * @return t_arg* The modified argument list.
 */
t_arg	 		*expand_args(t_arg *args, t_env *env, int exit);

/**
 * @brief Appends the value of a variable to the argument's new string.
 * @param arg The t_arg node.
 * @param i Current index in the original string.
 * @return char* The expanded string segment.
 */
char	 		*append_value(t_arg *arg, size_t i);

/**
 * @brief Appends the value of an environment variable.
 * @param arg The t_arg node.
 * @param i Current index in the original string.
 * @return char* The expanded string segment.
 */
char	 		*append_environment(t_arg *arg, size_t i);

/**
 * @brief Extracts and fetches the value of the environment variable starting at the dollar sign.
 * @param arg The t_arg node.
 * @param env The environment list.
 * @return char* The environment variable's value (deep copy) or an empty string.
 */
char	 		*get_expand_env(t_arg *arg, t_env *env);

/**
 * @brief Appends the shell's last exit code ($?) to the argument's new string.
 * @param arg The t_arg node.
 * @param exit_code The shell's last exit status.
 * @return char* The expanded string segment.
 */
char	 		*append_exit(t_arg *arg, int exit_code);

/**
 * @brief Appends a literal, unexpanded segment of the argument string.
 * @param arg The t_arg node.
 * @param i Current index in the original string.
 * @return char* The literal string segment.
 */
char	 		*append_literal(t_arg *arg, size_t i);

/**
 * @brief Expands environment variables and exit status in redirection filenames/delimiters.
 * @param redir The head of the t_redir list.
 * @param env The environment list.
 * @param exit The shell's last exit status.
 * @return t_redir* The modified redirection list.
 */
t_redir	 		*expand_redirs(t_redir *redir, t_env *env, int exit);

/**
 * @brief Appends a literal, unexpanded segment of the redirection file string.
 * @param redir The t_redir node.
 * @param i Current index in the original string.
 * @return char* The literal string segment.
 */
char	 		*redir_literal(t_redir *redir, size_t i);

/**
 * @brief Appends the shell's last exit code ($?) to the redirection file's new string.
 * @param redir The t_redir node.
 * @param exit The shell's last exit status.
 * @return char* The expanded string segment.
 */
char	 		*redir_exit(t_redir *redir, int exit);

/**
 * @brief Appends the value of a variable to the redirection file's new string.
 * @param redir The t_redir node.
 * @param i Current index in the original string.
 * @return char* The expanded string segment.
 */
char	 		*redir_value(t_redir *redir, size_t i);

/**
 * @brief Appends the value of an environment variable to the redirection file's new string.
 * @param redir The t_redir node.
 * @param i Current index in the original string.
 * @return char* The expanded string segment.
 */
char	 		*redir_environment(t_redir *redir, size_t i);

/**
 * @brief Extracts and fetches the value of the environment variable starting at the dollar sign in a redirection file.
 * @param redir The t_redir node.
 * @param env The environment list.
 * @return char* The environment variable's value (deep copy) or an empty string.
 */
char	 		*get_expand_redir(t_redir *redir, t_env *env);

#endif
```