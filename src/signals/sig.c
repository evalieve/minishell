/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sig.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:50 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/22 11:56:30 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// extern int	rl_catch_signals;
//	rl_catch_signals = 0; ????

// ctrl c = exit_status 1 > prints newline?
// ctrl d = exit_status 0 > in readline: if (!line) ?
// ctrl \ = doet niks > init SIGQUIT, SIG_INT

// init SIGSTOP, SIG_INT??


// ctrl c in heredoc?


void signal_ctrl_d(void)
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

// bij sigquit: exit status 131 - hoe kan ik dit (met waitpid, thanks copilot?) opvangen?
void	signal_child(int signum)
{
	if (signum == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
	// else if (signum == SIGQUIT)
	// 	signal(SIGQUIT, SIG_IGN);
	else if (signum == SIGQUIT)
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
}

void signal_parent(int signum) // dubbele prompt
{
   if (signum == SIGINT)
   {
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
   }
}

void signals(t_signal sig)
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
