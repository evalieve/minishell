/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/29 13:28:13 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*heredoc_loop(char *line, t_cmds *node)
{
	while (1)
	{
		line = readline("heredoc> ");
		if (line && ft_strcmp(line, node->redir->file) != 0)
		{
			ft_putstr_fd(line, node->pipe[1]);
			ft_putchar_fd('\n', node->pipe[1]);
		}
		else
			break ;
	}
	return (line);
}

void	handle_red_in1(t_cmds *node, t_redir *redir)
{
	char	*line;

	line = NULL;
	if (redir->type == RDIN)
		node->fd_in = ft_open(redir->file, O_RDONLY, 0644);
	else if (redir->type == RDHDOC)
	{
		ft_pipe(node->pipe);
		line = heredoc_loop(line, node);
		node->fd_in = node->pipe[0];
		ft_close(node->pipe[1]);
	}
}

void	handle_red_out1(t_cmds *node, t_redir *redir)
{
	if (redir->type == RDOUT)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RDAPPND)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_APPEND, 0644);
}

void	handle_redir(t_cmds *node)
{
	t_redir	*tmp;

	tmp = node->redir;
	while (tmp)
	{
		if (tmp->type == RDIN || tmp->type == RDHDOC)
		{
			if (node->fd_in != STDIN_FILENO)
				ft_close(node->fd_in);
			handle_red_in1(node, tmp);
			if (node->fd_in == ERROR)
			{
				if (node->fd_out != STDOUT_FILENO)
					return ;
			}
		}
		else if (tmp->type == RDOUT || tmp->type == RDAPPND)
		{
			if (node->fd_out != STDOUT_FILENO)
				ft_close(node->fd_out);
			handle_red_out1(node, tmp);
			if (node->fd_out == ERROR)
			{
				if (node->fd_in != STDIN_FILENO)
					ft_close(node->fd_in);
				return ;
			}
		}
		tmp = tmp->next;
	}
}
