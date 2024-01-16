/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fatal.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:54:41 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/16 16:07:44 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fatal(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	perror(NULL);
	exit(FAILURE);
}

void	non_fatal(char *str, char *pstr)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	perror(pstr);
}
