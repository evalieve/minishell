/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/30 18:30:05 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 18:34:39 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	handle_redir_in_loop(t_cmds *node, t_redir *tmp)
{
	if (node->fd_in != STDIN_FILENO)
		ft_close(node->fd_in);
	handle_red_in(node, tmp);
	if (node->fd_in == ERROR)
	{
		if (node->fd_out != STDOUT_FILENO)
			ft_close(node->fd_out);
		return (false);
	}
	return (true);
}

bool	handle_redir_out_loop(t_cmds *node, t_redir *tmp)
{
	if (node->fd_out != STDOUT_FILENO)
		ft_close(node->fd_out);
	handle_red_out(node, tmp);
	if (node->fd_out == ERROR)
	{
		if (node->fd_in != STDIN_FILENO)
			ft_close(node->fd_in);
		return (false);
	}
	return (true);
}
