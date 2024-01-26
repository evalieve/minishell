/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:40 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/26 19:24:20 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	builtin_pwd(t_cmds *cmd, t_minishell *minishell)
{
	// misschien gewoon pwd uit de env halen? ivm rm -rf
	char	*cwd;

	cwd = NULL;
	if (minishell->pwd)
		ft_putendl_fd(minishell->pwd, cmd->fd_out);
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			minishell->status = E_FAILURE;
			non_fatal("pwd", NULL);
		}
		else
		{
			printf("cwd: %s\n", cwd);
			ft_putendl_fd(cwd, cmd->fd_out);
			free(cwd);
		}
	}
	minishell->status = E_SUCCESS;
}
