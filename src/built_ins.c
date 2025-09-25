/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:31:42 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/09/25 11:26:00 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char *cmd, t_env *env, t_command *cmd_lst)
{
	
	if (!(ft_strncmp(cmd, "echo", 5)))
		return (ft_echo(cmd_lst));
	else if (!(ft_strncmp(cmd, "pwd", 4)))
		return (ft_pwd(env));
	else if (!(ft_strncmp(cmd, "env", 4)))
		return (ft_env(env));
	/* else if (!(ft_strncmp(cmd, "cd", 3)))
		return (ft_cd());
	else if (!(ft_strncmp(cmd, "export", 7)))
		return (ft_export());
	else if (!(ft_strncmp(cmd, "unset", 6)))
		return (ft_unset());
	else if (!(ft_strncmp(cmd, "exit", 5)))
		return (ft_exit()); */
	return (-1);
}

int	built_in(char *cmd, t_env *env, t_command *cmd_lst)
{
	pid_t	pid;
	int		exit_return;

	if (is_parent_built_in(cmd))
		return (execute_builtin(cmd, env, cmd_lst));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			exit_return = execute_builtin(cmd, env, cmd_lst);
			if (exit_return == -1)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			exit_return = ft_wait_and_exit(pid);
			signal(SIGINT, SIG_DFL);
			return (exit_return);
		}
			
	}
	return (-1);
}

/** @brief Prints PWD from env_list.
 * @warning No sé cómo debería actuar si PWD
 * está corrupto. Uso getcwd como backup pero
 * no sé si debería dejarlo romperse en paz.
 */
int	ft_pwd(t_env *env)
{
	char	*value;
	char	*cwd;

	if (!env)
		return (perror("env error"), EXIT_FAILURE);
	value = get_value_by_key(env, "PWD");
	if (value && access(value, F_OK) == 0) //si archivo o directorio existe
	{
		ft_printf("%s\n", value);
		free(value);
		return (EXIT_SUCCESS);
	}
	free(value);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("getcwd error"), EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}

/** @brief Shows in terminal the environment variables
 * stored in the t_env list.
 */
int	ft_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return (perror("env error"), EXIT_FAILURE);
	temp = env;
	while (temp)
	{
		ft_printf("%s\n", temp->full_env);
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/** @brief  Prints in STDOUT the parameters received.
 * If '-n' flag is given, there is no "\n" at the end.
*/
int	ft_echo(t_command *cmd)
{
	bool	newline;
	t_arg 	*temp;
	int		i;

	if (!cmd || ft_strncmp(cmd->args->value, "echo", 5))
		return (EXIT_FAILURE);
	temp = cmd->args->next;
	newline = true;
	while (temp && temp->value && ft_strncmp(temp->value, "-n", 2) == 0)
	{
		i = 2;
		while (temp->value[i] == 'n')
			i++;
		if (temp->value[i] != '\0')
			break;
		newline = false;
		temp = temp->next;
	}
	while (temp)
	{
		ft_printf("%s", temp->value);
		if (temp->next)
			ft_printf(" ");
		temp = temp->next;
	}
	if (newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}

/* int	ft_exit(char **args)
{
	int	status;

	ft_putstr_fd("exit\n", 2);
	if (!args[1])
		exit(0);
	else if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1); // bash no sale si hay muchos argumentos
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(255);
	}
	status = ft_atoi(args[1]) % 256;
	exit(status);
}

int	ft_unset(char **args, t_env **env)
{
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		delete_env_key(env, args[i]); // debes tener una función que libere el nodo
		i++;
	}
	return (0);
}

int	ft_export(char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_sorted_env(*env); // muestra env tipo export
		return (0);
	}
	i = 1;
	while (args[i])
	{
		key = set_key(args[i]);
		value = set_value(args[i]);
		if (!key)
			return (ft_putstr_fd("export: invalid identifier\n", 2), 1);
		update_env_value(env, key, value); // crea o modifica
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (0);
}

int	ft_cd(char **args, t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!args[1]) // cd sin argumentos → ir a HOME
	{
		new_pwd = get_value_by_key(*env, "HOME");
		if (!new_pwd)
			return (ft_putstr_fd("cd: HOME not set\n", 2), free(old_pwd), 1);
	}
	else if (ft_strncmp(args[1], "-", 2) == 0) // cd -
	{
		new_pwd = get_value_by_key(*env, "OLDPWD");
		if (!new_pwd)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), free(old_pwd), 1);
		ft_printf("%s\n", new_pwd); // imprimir nuevo path como hace bash
	}
	else
		new_pwd = ft_strdup(args[1]);

	if (chdir(new_pwd) != 0)
	{
		perror("cd");
		free(old_pwd);
		free(new_pwd);
		return (1);
	}
	update_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);

	old_pwd = getcwd(NULL, 0);
	update_env_value(env, "PWD", old_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
} */
