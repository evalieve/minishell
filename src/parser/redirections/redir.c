/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 11:00:51 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// heredoc signal! TODO
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

void	handle_red_in(t_cmds *node, t_redir *redir)
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

void	handle_red_out(t_cmds *node, t_redir *redir)
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
			if (!handle_redir_in_loop(node, tmp))
				return ;
		}
		else if (tmp->type == RDOUT || tmp->type == RDAPPND)
		{
			if (!handle_redir_out_loop(node, tmp))
				return ;
		}
		tmp = tmp->next;
	}
}
