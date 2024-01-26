/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 16:13:17 by evalieve      ########   odam.nl         */
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

// void	handle_red_in(t_cmds *node)
// {
// 	char	*line;

// 	line = NULL;
// 	node->fd_in = 0;
// 	while (node->redir)
// 	{
// 		if (node->redir->type == RDIN)
// 			node->fd_in = open(node->redir->file, O_RDONLY, 0644);
// 		else if (node->redir->type == RDHDOC)
// 		{
// 			pipe(node->pipe);
// 			line = heredoc_loop(line, node);
// 			node->fd_in = node->pipe[0];
// 			close(node->pipe[1]);
// 		}
// 		if (node->redir->next && node->redir->type != RDHDOC)
// 			close(node->fd_in);
// 		node->redir = node->redir->next;
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
	// if (node->fd_in == ERROR) // dit slaat nergens op, want hij returned sws al hierna en check is in andere func
		// return ; // hij moet stoppen met de rest van de files openen na error met permissions
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
	// if (node->fd_out == ERROR) // dit slaat nergens op, want hij returned sws al hierna en check is in andere func
	// 	return ; // hij moet stoppen met de rest van de files openen na error met permissions
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
				if (node->fd_out != STDOUT_FILENO) // bij error moet hij ook de out file sluiten mits deze bestaat en geopend is
					ft_close(node->fd_out);
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
				if (node->fd_in != STDIN_FILENO) // bij error moet hij ook de in file sluiten mits deze bestaat en geopend is
					ft_close(node->fd_in);
				// printf("error RDOUT\n");
				return ; // hij moet stoppen met de rest van de files openen na error met permissions
			}
		}
		tmp = tmp->next;
	}
// 	handle_red_in(node);
// 	handle_red_out(node);
}

// void	handle_red_out(t_cmds *node)
// {
// 	while (node->redir && (node->redir->type == RDOUT || \
// 	node->redir->type == RDAPPND))
// 	{
// 		if (node->redir->type == RDOUT)
// 			node->fd_out = open(node->redir->file, \
// 			O_RDWR | O_CREAT | O_TRUNC, 0644);
// 		else if (node->redir->type == RDAPPND)
// 			node->fd_out = open(node->redir->file, \
// 			O_RDWR | O_CREAT | O_APPEND, 0644);
// 		if (node->redir->next)
// 			ft_close(node->fd_out);
// 		node->redir = node->redir->next;
// 	}
// }

// void	handle_redir(t_cmds *node)
// {
// 	handle_red_in(node);
// 	handle_red_out(node);
// }
