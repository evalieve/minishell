/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2023/12/21 14:02:59 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_red_in(t_cmds *node)
{
	char *line;

	line = NULL;
	node->fd_in = 0;
	while (node->in)
	{
		if (node->in->type == RDIN)
			node->fd_in = ft_open(node->in->file, O_RDONLY, 0644);
		else if (node->in->type == RDHDOC)
		{
			ft_pipe(node->pipe);
			while (1)
			{
				line = readline("heredoc> ");
				if (line && ft_strncmp(line, node->in->file, ft_strlen(line)) != 0)
					ft_putstr_fd(line, node->pipe[1]);
				else
					break ;
			}
			node->fd_in = node->pipe[0];
			ft_close(node->pipe[1]);
		}
		node->in = node->in->next;
	}
}

void	handle_red_out(t_cmds *node)
{
	while (node->out && (node->out->type == RDOUT || node->out->type == RDAPPND))
	{
		if (node->out->type == RDOUT)
			node->fd_out = ft_open(node->out->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else if (node->out->type == RDAPPND)
			node->fd_out = ft_open(node->out->file, O_RDWR | O_CREAT | O_APPEND, 0644);
		node->out = node->out->next;
	}
}
​