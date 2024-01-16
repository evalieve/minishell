/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:40 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 15:39:42 by evalieve      ########   odam.nl         */
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
		write(STDERR_FILENO, "minishell: pwd: ", 17);
		perror(NULL);
	}
	else
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		free(cwd);
	}
	minishell->status = E_SUCCESS;
}
