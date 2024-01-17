/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:31 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/17 02:40:55 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_flag(char *arg)
{
	int	i;

	i = 2;
	if (arg[0] == '-' && arg[1] == 'n')
	{
		while (arg[i] && arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (true);
	}
	return (false);
}

void	builtin_echo(t_cmds *cmd, t_minishell *minishell)
{
	bool	flag;
	int		fd;
	int		i;

	flag = false;
	fd = get_fd_out(cmd); // waarom niet gewoon fd = cmd->fd_out?
	i = 1;
	if (!cmd->args[i])
		return (ft_putchar_fd('\n', fd));
	if (is_flag(cmd->args[i]))
	{
		flag = true;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], fd);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', fd);
	minishell->status = E_SUCCESS;
}
