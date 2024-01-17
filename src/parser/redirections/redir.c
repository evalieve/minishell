/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/17 02:07:46 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*heredoc_loop(char *line, t_cmds *node)
{
	while (1)
	{
		line = readline("heredoc> ");
		if (line && ft_strncmp(line, node->redir->file, ft_strlen(line)) != 0)
		{
			ft_putstr_fd(line, node->pipe[1]);
			ft_putchar_fd('\n', node->pipe[1]);
		}
		else
			break ;
	}
	return (line);
}

// void	handle_red_in(t_cmds *node)
// {
// 	char	*line;

// 	line = NULL;
// 	node->fd_in = 0;
// 	while (node->in)
// 	{
// 		if (node->in->type == RDIN)
// 			node->fd_in = ft_open(node->in->file, O_RDONLY, 0644);
// 		else if (node->in->type == RDHDOC)
// 		{
// 			ft_pipe(node->pipe);
// 			line = heredoc_loop(line, node);
// 			node->fd_in = node->pipe[0];
// 			ft_close(node->pipe[1]);
// 		}
// 		if (node->fd_in == ERROR)
// 			return ; // hij moet stoppen met de rest van de files openen na error met permissions
// 		if (node->in->next)
// 			ft_close(node->fd_in);
// 		node->in = node->in->next;
// 	}
// }

// void	handle_red_out(t_cmds *node)
// {
// 	while (node->out && (node->out->type == RDOUT || \
// 	node->out->type == RDAPPND))
// 	{
// 		if (node->out->type == RDOUT)
// 			node->fd_out = ft_open(node->out->file, \
// 			O_RDWR | O_CREAT | O_TRUNC, 0644);
// 		else if (node->out->type == RDAPPND)
// 			node->fd_out = ft_open(node->out->file, \
// 			O_RDWR | O_CREAT | O_APPEND, 0644);
// 		if (node->fd_out == ERROR)
// 			return ; // hij moet stoppen met de rest van de files openen na error met permissions
// 		if (node->out->next)
// 			ft_close(node->fd_out);
// 		node->out = node->out->next;
// 	}
// }

void	handle_red_in1(t_cmds *node, t_redir *redir)
{
	char	*line;

	line = NULL;
	// printf("\nnode->redir->type = %d\n", node->redir->type);
	if (redir->type == RDIN)
		node->fd_in = ft_open(redir->file, O_RDONLY, 0644);
	else if (redir->type == RDHDOC)
	{
		ft_pipe(node->pipe);
		line = heredoc_loop(line, node);
		node->fd_in = node->pipe[0];
		ft_close(node->pipe[1]);
	}
	// printf("node->fd_in in handle_ret_in1 = %d\n", node->fd_in);
	if (node->fd_in == ERROR)
		return ; // hij moet stoppen met de rest van de files openen na error met permissions
	// if (node->in->next)
	// 	ft_close(node->fd_in);
}

void	handle_red_out1(t_cmds *node, t_redir *redir)
{
	// printf("\nnode->redir->type = %d\n", node->redir->type);
	if (redir->type == RDOUT)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RDAPPND)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_APPEND, 0644);
	// printf("node->fd_out in handle_ret_out1 = %d\n", node->fd_out);
	if (node->fd_out == ERROR)
		return ; // hij moet stoppen met de rest van de files openen na error met permissions
	// if (node->redir->next)
	// 	ft_close(node->fd_out);
}

void	handle_redir(t_cmds *node)
{
	t_redir	*tmp;

	tmp = node->redir;
	while (tmp)
	{
		if (tmp->type == RDIN || tmp->type == RDHDOC)
		{
			// printf("\n========\nnode->fd_in = %d, type = %d\n", node->fd_in, tmp->type);
			if (node->fd_in != STDIN_FILENO)
				ft_close(node->fd_in);
			handle_red_in1(node, tmp);
			if (node->fd_in == ERROR)
			{
				// printf("error RDIN\n");
				return ; // hij moet stoppen met de rest van de files openen na error met permissions
			}
		}
		else if (tmp->type == RDOUT || tmp->type == RDAPPND)
		{
			// printf("\n=========\nnode->fd_out = %d\n", node->fd_out);
			if (node->fd_out != STDOUT_FILENO)
				ft_close(node->fd_out);
			handle_red_out1(node, tmp);
			if (node->fd_out == ERROR)
			{
				// printf("error RDOUT\n");
				return ; // hij moet stoppen met de rest van de files openen na error met permissions
			}
		}
		tmp = tmp->next;
	}
// 	handle_red_in(node);
// 	handle_red_out(node);
}
