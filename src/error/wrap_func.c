/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrap_func.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 16:48:42 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 10:56:54 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		fatal("malloc", NULL);
		return (NULL);
	}
	return (ptr);
}

int	ft_close(int fd)
{
	if (close(fd) == ERROR)
	{
		fatal("close", NULL);
		return (ERROR);
	}
	return (SUCCESS);
}

int	ft_open(char *file, int flag, int mode)
{
	int	fd;

	fd = open(file, flag, mode);
	if (fd == ERROR)
		non_fatal(file, NULL);
	return (fd);
}
