/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sig.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:50 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/01 14:19:13 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signal_ctrl_d(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	minishell->exit = 1;
}

void	signal_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		exit(E_SIGINT);
	}
}

void	signal_child(int signum)
{
	if (signum == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
	else if (signum == SIGQUIT)
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
}

void	signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signals(t_signal sig)
{
	if (sig == S_PARENT)
	{
		signal(SIGINT, &signal_parent);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig == S_CHILD)
	{
		signal(SIGINT, signal_child);
		signal(SIGQUIT, signal_child);
	}
	else if (sig == S_HEREDOC)
	{
		signal(SIGINT, signal_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig == S_IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}
