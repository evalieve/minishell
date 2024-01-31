/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:24:25 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/31 11:34:12 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
