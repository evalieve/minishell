/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/30 18:30:05 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/01 01:55:28 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	handle_redir_in_loop(t_cmds *node, t_redir *tmp, t_minishell *mini)
{
	int	e_status;

	if (node->fd_in != STDIN_FILENO)
		ft_close(node->fd_in);
	e_status = handle_red_in(node, tmp, mini);
	if (e_status == E_SIGINT || e_status == E_FAILURE)
	{
		if (node->fd_out != STDOUT_FILENO)
			ft_close(node->fd_out);
	}
	return (e_status);
}

int	handle_redir_out_loop(t_cmds *node, t_redir *tmp)
{
	int	e_status;

	if (node->fd_out != STDOUT_FILENO)
		ft_close(node->fd_out);
	e_status = handle_red_out(node, tmp);
	if (e_status == E_FAILURE)
	{
		if (node->fd_in != STDIN_FILENO)
			ft_close(node->fd_in);
	}
	return (e_status);
}
