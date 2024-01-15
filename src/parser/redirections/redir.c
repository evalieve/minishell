/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/15 16:03:28 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*heredoc_loop(char *line, t_cmds *node)
{
	while (1)
	{
		line = readline("heredoc> ");
		if (line && ft_strncmp(line, node->in->file, ft_strlen(line)) != 0)
		{
			ft_putstr_fd(line, node->pipe[1]);
			ft_putchar_fd('\n', node->pipe[1]);
		}
		else
			break ;
	}
	return (line);
}

void	handle_red_in(t_cmds *node)
{
	char	*line;

	line = NULL;
	node->fd_in = 0;
	while (node->in)
	{
		if (node->in->type == RDIN)
			node->fd_in = open(node->in->file, O_RDONLY, 0644);
		else if (node->in->type == RDHDOC)
		{
			pipe(node->pipe);
			line = heredoc_loop(line, node);
			node->fd_in = node->pipe[0];
			close(node->pipe[1]);
		}
		if (node->in->next)
			ft_close(node->fd_in);
		node->in = node->in->next;
	}
}

void	handle_red_out(t_cmds *node)
{
	while (node->out && (node->out->type == RDOUT || \
	node->out->type == RDAPPND))
	{
		if (node->out->type == RDOUT)
			node->fd_out = open(node->out->file, \
			O_RDWR | O_CREAT | O_TRUNC, 0644);
		else if (node->out->type == RDAPPND)
			node->fd_out = open(node->out->file, \
			O_RDWR | O_CREAT | O_APPEND, 0644);
		if (node->out->next)
			ft_close(node->fd_out);
		node->out = node->out->next;
	}
}

void	handle_redir(t_cmds *node)
{
	handle_red_in(node);
	handle_red_out(node);
}
