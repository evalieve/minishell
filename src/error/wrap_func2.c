/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrap_func2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:53:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/08 17:54:11 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		fatal("fork ");
	return (pid);
}

void	ft_pipe(int *p)
{
	if (pipe(p) == -1)
		fatal("pipe ");
}

pid_t	ft_waitpid(pid_t pid, int *status, int option)
{
	pid_t	rv;

	rv = waitpid(pid, status, option);
	if (rv == -1)
		fatal("waitpid ");
	return (rv);
}

int	ft_dup2(int fd1, int fd2)
{
	int	fd;

	fd = 0;
	if (fd1 != -1 && fd2 != -1)
	{
		fd = dup2(fd1, fd2);
		if (fd == -1)
			fatal("dup2 ");
	}
	return (fd);
}
