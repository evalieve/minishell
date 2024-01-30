/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sig.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:50 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 13:34:46 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signal_ctrl_d(void)
{
	write(1, "exit\n", 6);
	exit(0);
}

void	signal_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
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
}
