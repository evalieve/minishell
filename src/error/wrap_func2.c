/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrap_func2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:53:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 18:55:37 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		fatal("fork", NULL);
	return (pid);
}

void	ft_pipe(int *p)
{
	if (pipe(p) == ERROR)
		fatal("pipe", NULL);
}

pid_t	ft_waitpid(pid_t pid, int *status, int option)
{
	pid_t	rv;

	rv = waitpid(pid, status, option);
	if (rv == ERROR)
		fatal("waitpid", NULL);
	return (rv);
}

int	ft_dup2(int fd1, int fd2)
{
	if (fd1 == ERROR || fd2 == ERROR)
		return (ERROR);
	if (dup2(fd1, fd2) == ERROR)
		fatal("dup2", NULL);
	return (SUCCESS);
}

int	ft_execve(char *path, char **argv, char **envp)
{
	int	rv;

	rv = execve(path, argv, envp);
	if (rv == ERROR)
		fatal(path, NULL);
	return (rv);
}
