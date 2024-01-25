/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrap_func.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 16:48:42 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/25 16:58:32 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void *ft_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		fatal("malloc ");
		return (NULL);
	}
	return (ptr);
}

int ft_close(int fd)
{
	if (close(fd) == -1)
	{
		fatal("close ");
		return (-1);
	}
	return (0);
}

int ft_open(char *file, int flag, int mode)
{
	int fd;

	fd = open(file, flag, mode);
	if (fd == -1)
	{
		non_fatal("open ");
		return (-1);
	}
	return (fd);
}
