/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:40 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/25 15:12:05 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	builtin_pwd(t_cmds *cmd, t_minishell *minishell)
{
	char	*cwd;

	cmd = (void *)cmd;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		minishell->status = E_FAILURE;
		non_fatal("pwd", NULL);
	}
	else
	{
		ft_putendl_fd(cwd, cmd->fd_out);
		free(cwd);
	}
	minishell->status = E_SUCCESS;
}
