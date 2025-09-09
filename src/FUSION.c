/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FUSION.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:33:23 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/09 17:17:19 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**command_to_arr(t_command *command)
{
	char	**command_array;
	t_arg	*temp;
	size_t	i;

	if (!command)
		return (NULL);
	command_array = NULL;
	temp = command->args;
	i = 0;
	while (temp)
	{
		command_array[i] = ft_strdup(temp->value);
		temp = temp->next;
	}
	command_array[i] = NULL;
	return (command_array);
}

void	execute_command(t_command *command, t_env **envlist, int in, int out)
{
	char	**args;
	char	*full_path;
	char	**env_arr;

	env_arr = envlist_to_arr(envlist);
	if (!env_arr)
		return ;
	args = command_to_arr(command);
	if (!args || !args[0])
	{
		ft_free_array(env_arr);
		return ;
	}
	full_path = ft_check_path(args[0], env_arr);
	if (!full_path)
		cmd_not_found(args[0], args);
	execve(full_path, args, env_arr);
	perror("execve");
	ft_free_array(args);
	ft_free_array(env_arr);
	free(full_path);
	exit (1);
}

int	command_in(t_command *command, t_env **environment, int in, int out)
{
	pid_t	pid;
	pid_t	last_pid;
	size_t	redirs_count;
	t_redir	*redir;
	int		new_in;
	int		new_out;

	if (!command || !environment)
		return (perror("No data"), EXIT_FAILURE);
	pid = -1;
	last_pid = -1;
	redirs_count = number_of_redirs(command);
	redir = command->redirs;
	if (!redir)
	{
		pid = fork();
		if (pid < 0)
			return (perror("Fork"), EXIT_FAILURE);
		if (pid == 0)
			execute_command(command, environment, in, out);
		return (ft_wait_and_exit(pid));
	}
	while(redir)
	{
		pid = fork();
		if (pid < 0)
			return (perror("Fork"), EXIT_FAILURE);
		if (pid == 0)
		{
			new_in = redirect_in(command, in);
			new_out = redirect_out(command, out);
			if (in != new_in)
			{
				if (dup2(new_in, in) == -1)
					return(perror("dup2 in"), EXIT_FAILURE);
				close(in);
			}
			if (out != new_out)
			{
				if (dup2(new_out, out) == -1)
					return(perror("dup2 out"), EXIT_FAILURE);
				close(out);
			}
			execute_command(command, environment, new_in, new_out);
		}
		last_pid = pid;
		redir = redir->next;
	}
	return (ft_wait_and_exit(last_pid));
}

/// @brief Initiate a child process to execute a given command
int	execute_all(t_command *commands, t_env **environment)
{
	size_t	command_count;
	t_pipe	*pipe_data;
	int		exit_signal;

	if (!commands || !environment)//Qué error sería este?
		return (perror("Missing data structures"), EXIT_FAILURE);
	command_count = number_of_commands(commands);
	if (command_count < 1)
		return (perror("No command"), EXIT_FAILURE);
	if (command_count == 1)
		return (command_in(commands, environment, STDIN_FILENO, STDOUT_FILENO));
	pipe_data = init_pipe_data(commands, environment, command_count);
	if (!pipe_data)
		return (perror("pipe_data error"), EXIT_FAILURE);
	exit_signal = pipes(pipe_data);
	free_pipe_data(pipe_data);
	return (exit_signal);
}

static void	child_process(t_pipe *pipe_data, int prev_pipe, int *pipe_fd)
{
	if (pipe_data->index == 0)//si es el primero, usar in
	{
		if (dup2(pipe_data->in, 0) == -1)
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	else//si no, usar anterior pipe
	{
		if (dup2(prev_pipe, 0) == -1)
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	if (pipe_data->index == pipe_data->command_count - 1)
	{
		if (dup2(pipe_data->out, 1) == -1)// si es el último comando, escribir en out
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	else
	{
		if (dup2(pipe_fd[1], 1) == -1)// si no, escribir en pipe actual para que el siguiente proceso lo lea
			perror("dup2 in"), exit(EXIT_FAILURE);
	}
	ft_close_three(pipe_data->in, pipe_data->out, prev_pipe);
	if (pipe_data->index < pipe_data->command_count - 1)//si no estamos en el último comando, cerrar pipe actual
		ft_close_two(pipe_fd[0], pipe_fd[1]);
	command_in(pipe_data->commands, pipe_data->env_list, pipe_data->in, pipe_data->out);
	exit(EXIT_FAILURE);
}

static void	next_pipe(t_pipe *pipe_data, int *pipe_fd, int *prev_pipe)
{
	if (pipe_data->index < pipe_data->command_count - 1)
	{
		close(pipe_fd[1]); //ya hemos escrito aquí
		*prev_pipe = pipe_fd[0]; //de aquí leerá el próximo comando
	}
	else
	{
		if (pipe_fd[0] != -1 && pipe_fd[1] != -1)
			ft_close_two(pipe_fd[0], pipe_fd[1]);// Último comando, ya no necesitamos pipe_fds
	}
	return ;
}

/// @brief Pipe manager:
int	pipes(t_pipe *pipe_data)
{
	int		pipe_fd[2];
	int		prev_pipe;
	pid_t	pid;
	pid_t	last_pid;

	prev_pipe = -1;
	last_pid = -1;
	while (pipe_data->index < pipe_data->command_count)
	{
		if (pipe_data->index < pipe_data->command_count - 1 && pipe(pipe_fd) == -1) // si no estamos en el último comando(blocks -1), necesitamos pipe
			return (perror("pipe"), EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
			child_process(pipe_data, prev_pipe, pipe_fd);
		last_pid = pid;
		safe_close(prev_pipe); //cerrar descriptor que ya ha heredado el hijo
		next_pipe(pipe_data, pipe_fd, &prev_pipe);
		pipe_data->index ++;
		pipe_data->commands = pipe_data->commands->next;
	}
	ft_close_two(pipe_data->in, pipe_data->out);
	return (ft_wait_and_exit(last_pid));
}

size_t	number_of_commands(t_command *command_list)
{
	t_command	*temp;
	size_t		count;

	if (!command_list)
		return (0);
	temp = command_list;
	count = 0;
	while (temp)
	{
		temp = temp->next;
		count ++;
	}
	return (count);
}

int	redirect_in(t_command *command_list, int in_fd)
{
	if (command_list->redirs->type == REDIR_IN)
		in_fd = get_inputfile_fd(command_list->redirs->file);
	else if (command_list->redirs->type == HEREDOC)
		in_fd = get_heredoc_fd(command_list->redirs->file);
	return (in_fd);
}

int	redirect_out(t_command *command_list, int out_fd)
{
	if (command_list->redirs->type == REDIR_OUT)
		out_fd = get_outputfile_fd(command_list->redirs->file);
	else if (command_list->redirs->type == APPEND)
		out_fd = get_append_fd(command_list->redirs->file);
	return (out_fd);
}

size_t	number_of_redirs(t_command *command_list)
{
	size_t	count;
	t_redir	*temp;

	if (!command_list)
		return (0);
	if (!command_list->redirs)
		return (0);
	temp = command_list->redirs;
	count = 0;
	while(temp)
	{
		temp = command_list->redirs->next;
		count ++;
	}
	return (count);
}
