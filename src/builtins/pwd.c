/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:40 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/18 15:34:29 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
// #include "../include/minishell.h"


void	builtin_pwd(t_cmds *cmd, t_minishell *minishell)
{
	char	*cwd;

	cmd = (void*)cmd;
	cwd = getcwd(NULL, 0); // kan ook uit env halen?
	if (!cwd)
	{
		write(STDERR_FILENO, "minishell: pwd: ", 17);
		minishell->status = E_FAILURE;
		perror(NULL);
	}
	else
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		free(cwd);
	}
	minishell->status = E_SUCCESS;
}
