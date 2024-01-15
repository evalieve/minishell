/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fatal.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:54:41 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/15 16:15:12 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fatal(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	perror(NULL);
	exit(FAILURE);
}

void	non_fatal(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	perror(NULL);
}
