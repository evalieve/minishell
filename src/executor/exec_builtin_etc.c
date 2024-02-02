/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:24:25 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/02 14:12:44 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_command_path_error(t_minishell *mini, t_cmds *cmd, char *path)
{
	if ((!path && key_exist(mini->env, "PATH")) \
		|| empty_input(cmd->cmd) == true)
	{
		if (path)
			free(path);
		error_message(cmd->cmd, NULL, "command not found");
		exit(E_COMMAND_NOT_FOUND);
	}
	if (access(path, F_OK) == ERROR)
	{
		if (path)
			free(path);
		error_message(cmd->cmd, NULL, "No such file or directory");
		exit(E_NO_SUCH_FILE_OR_DIRECTORY);
	}
	if (access(path, X_OK) == ERROR)
	{
		if (path)
			free(path);
		error_message(cmd->cmd, NULL, "Permission denied");
		exit(E_NO_SUCH_FILE_OR_DIRECTORY);
	}
}

t_builtin	builtin_lookup(char *cmd)
{
	int						i;
	static const t_builtin	lookup[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"exit", builtin_exit},
	{NULL, NULL}
	};

	i = 0;
	while (lookup[i].cmd && ft_strcmp(lookup[i].cmd, cmd))
		i++;
	return (lookup[i]);
}

void	exec_builtin(t_cmds *cmd, t_minishell *minishell)
{
	t_builtin	builtin;

	builtin = builtin_lookup(cmd->cmd);
	builtin.func(cmd, minishell);
}
