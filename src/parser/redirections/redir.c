/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/21 13:55:31 by marlou        #+#    #+#                 */
/*   Updated: 2024/02/01 14:29:50 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	wait_for_heredoc(pid_t pid, t_minishell *minishell)
{
	int	e_status;

	ft_waitpid(pid, &e_status, 0);
	signals(S_PARENT);
	if (WEXITSTATUS(e_status) == E_SIGINT)
	{
		minishell->status = E_FAILURE;
		return (E_SIGINT);
	}
	minishell->status = E_SUCCESS;
	return (E_SUCCESS);
}

int	heredoc_loop(t_cmds *node, t_minishell *minishell)
{
	char	*line;
	pid_t	pid;

	line = NULL;
	signals(S_IGNORE);
	pid = ft_fork();
	if (pid == CHILD)
	{
		signals(S_HEREDOC);
		while (1)
		{
			line = readline("heredoc> ");
			if (line && ft_strcmp(line, node->redir->file) != SUCCESS)
			{
				ft_putstr_fd(line, node->pipe[1]);
				ft_putchar_fd('\n', node->pipe[1]);
			}
			else
				exit(E_CTRL_D);
		}
	}
	return (wait_for_heredoc(pid, minishell));
}

int	handle_red_in(t_cmds *node, t_redir *redir, t_minishell *minishell)
{
	int	e_status;

	e_status = E_SUCCESS;
	if (redir->type == RDIN)
	{
		node->fd_in = ft_open(redir->file, O_RDONLY, 0644);
		if (node->fd_in == ERROR)
			e_status = E_FAILURE;
	}
	else if (redir->type == RDHDOC)
	{
		ft_pipe(node->pipe);
		e_status = heredoc_loop(node, minishell);
		node->fd_in = node->pipe[0];
		ft_close(node->pipe[1]);
	}
	return (e_status);
}

int	handle_red_out(t_cmds *node, t_redir *redir)
{
	int	e_status;

	e_status = E_SUCCESS;
	if (redir->type == RDOUT)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RDAPPND)
		node->fd_out = ft_open(redir->file, \
		O_RDWR | O_CREAT | O_APPEND, 0644);
	if (node->fd_out == ERROR)
		e_status = E_FAILURE;
	return (e_status);
}

int	handle_redir(t_cmds *node, t_minishell *minishell)
{
	t_redir	*tmp;
	int		e_status;

	tmp = node->redir;
	while (tmp)
	{
		if (tmp->type == RDIN || tmp->type == RDHDOC)
		{
			e_status = handle_redir_in_loop(node, tmp, minishell);
			if (e_status != E_SUCCESS)
				return (e_status);
		}
		else if (tmp->type == RDOUT || tmp->type == RDAPPND)
		{
			e_status = handle_redir_out_loop(node, tmp);
			if (e_status != E_SUCCESS)
				return (e_status);
		}
		tmp = tmp->next;
	}
	return (E_SUCCESS);
}
