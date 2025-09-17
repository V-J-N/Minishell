/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:50:20 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/17 13:05:22 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_input_redir(t_command *cmd)
{
    t_redir *r = cmd->redirs;
    while (r)
    {
        if (r->type == REDIR_IN || r->type == HEREDOC)
            return true;
        r = r->next;
    }
    return false;
}

bool	has_output_redir(t_command *cmd)
{
    t_redir *r = cmd->redirs;
    while (r)
    {
        if (r->type == REDIR_OUT || r->type == APPEND)
            return true;
        r = r->next;
    }
    return false;
}

bool	has_redirs(t_command *cmd)
{
	return (cmd && cmd->redirs != NULL);
}

/* 
IMPORTANTE: Al parecer, si bloque de comando tiene varias redirecciones,
no utiliza procesos hijos por separado, sino que se ejecuta en un
solo proceso, escribiendo a la vez en varios FDs!
En shells reales, cada comando con sus redirecciones se ejecuta en un proceso.
Hace falta implementar o testear múltiples redirecciones en un mismo
bloque de comandos
 */
int redirect_in(t_command *command_list, int in_fd)
{
	t_redir *temp;
	int 	new_fd;

	temp = command_list->redirs;
	new_fd = in_fd;
	while (temp)
	{
		if (temp->type == REDIR_IN)
		 {
            if (new_fd != STDIN_FILENO)
                close(new_fd);
            new_fd = get_inputfile_fd(temp->file);
            if (new_fd == -1)
                return -1;
        }
		else if (temp->type == HEREDOC)
		{
            if (new_fd != STDIN_FILENO)
                close(new_fd);
            new_fd = temp->heredoc_fd;
        }
		temp = temp->next;
	}
	return (new_fd);
}

int redirect_out(t_command *command_list, int out_fd)
{
	t_redir *temp;
	int 	new_fd;

	temp = command_list->redirs;
	new_fd = out_fd;
	while (temp)
	{
		if (temp->type == REDIR_OUT)
		{
			if (new_fd != STDOUT_FILENO)
    			close(new_fd);
			new_fd = get_outputfile_fd(temp->file);
			if (new_fd == -1)
                return -1;
		}
		else if (temp->type == APPEND)
		{
            if (new_fd != STDOUT_FILENO)
                close(new_fd);
            new_fd = get_append_fd(temp->file);
            if (new_fd == -1)
                return -1;
        }
		temp = temp->next;
	}
	return (new_fd);
}

int	get_inputfile_fd(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
		return (perror(infile), -1);
	return (fd);
}

int	get_outputfile_fd(char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);//abrir outfile con opción de reescritura (truncate)
	if (fd < 0)
		return (perror(outfile), -1);
	return (fd);
}

int	get_append_fd(char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);//abrir outfile con opción de append
	if (fd < 0)
		return (perror(outfile), -1);
	return (fd);
}

int	redirection_only(t_command *cmd, int in, int out)
{
	pid_t	pid;
	int		new_in, new_out;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("Fork"), EXIT_FAILURE);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		new_in = redirect_in(cmd, in);
		new_out = redirect_out(cmd, out);
		if (new_in == -1 || new_out == -1)
			exit(EXIT_FAILURE);
		if (new_in != STDIN_FILENO)
		{
			dup2(new_in, STDIN_FILENO);
			close(new_in);
		}
		if (new_out != STDOUT_FILENO)
		{
			dup2(new_out, STDOUT_FILENO);
			close(new_out);
		}
		// No ejecutamos comando. Solo redirecciones.
		exit(EXIT_SUCCESS);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	return (ft_wait_and_exit(status));
}

bool prepare_all_heredocs(t_command *cmd_list)
{
	t_command *current;

	current = cmd_list;
	while (current)
	{
		if (!prepare_heredocs(current))
			return (false);
		current = current->next;
	}
	return (true);
}

bool prepare_heredocs(t_command *cmd)
{
	t_redir	*redir;
	int		fd;

	if (!cmd)
		return (true);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			fd = get_heredoc_fd(redir->file);
			if (fd == -1)
				return (false);
			redir->heredoc_fd = fd;
		}
		redir = redir->next;
	}
	return (true);
}

static void	heredoc_child_process(char *limiter, int *pipe_fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL); // restaurar comportamiento default (terminar con Ctrl+C)
	close(pipe_fd[0]); // cerramos lectura en el hijo
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			exit(EXIT_SUCCESS); // EOF o Ctrl+D
		if (strncmp(line, limiter, strlen(limiter)) == 0 && line[strlen(limiter)] == '\n')
		{
			free(line);
			break;
		}
		if (write(pipe_fd[1], line, strlen(line)) == -1)
		{
			perror("write to pipe");
			free(line);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

/* HERE_DOC!!:
Si presionas Ctrl+C mientras estás escribiendo en el heredoc, lo correcto es que el shell:
Interrumpa solo el heredoc.
NO termine todo el minishell.
Regrese al prompt de comandos, como hace bash/zsh.

Solución: usar fork()
Separar el heredoc en un proceso hijo permite:
Que el hijo maneje la lectura.
Que si se presiona Ctrl+C, sólo se mate el hijo, no el minishell.
El padre puede detectar la interrupción y actuar (por ejemplo, no ejecutar el comando).
Lagunas teóricas: no entiendo muy bien cómo funcionan estas señales!
 */
int	get_heredoc_fd(char *limiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)//creamos pipe
		return (perror("here_doc pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0) // HIJO:
		heredoc_child_process(limiter, pipe_fd);
	signal(SIGINT, SIG_IGN); // ignorar SIGINT en el padre durante el heredoc
	close(pipe_fd[1]); // cerramos escritura en el padre
	waitpid(pid, &status, 0); // esperamos que termine el hijo
	signal(SIGINT, SIG_DFL); // restauramos manejo de SIGINT
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)// El heredoc fue interrumpido con Ctrl+C
		return (close(pipe_fd[0]), -1);
	return (pipe_fd[0]); // devuelve el fd de lectura con el contenido
}
