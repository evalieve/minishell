/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:33 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 15:26:15 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
// #include "../../include/minishell.h"

void	print_env_line(t_env *env, int fd)
{
	ft_putstr_fd(env->key, fd);
	if (!env->equal_sign)
	{
		ft_putchar_fd('\n', fd);
		return ;
	}
	if (env->equal_sign)
		ft_putchar_fd('=', fd);
	ft_putchar_fd('"', fd);
	if (env->value)
		ft_putstr_fd(env->value, fd);
	ft_putchar_fd('"', fd);
	ft_putchar_fd('\n', fd);
}

void	builtin_env(t_cmds *cmd, t_minishell *minishell)
{
	t_env	*ptr;
	int		fd;

	ptr = minishell->env;
	fd = get_fd_out(cmd);
	while (ptr)
	{
		if (ptr->key && ptr->equal_sign)
			print_env_line(ptr, fd);
		ptr = ptr->next;
	}
	minishell->status = E_SUCCESS;
}
