/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sig.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:50 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/21 15:07:31 by marlou        ########   odam.nl         */
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
	write(1, " exit\n", 7);
	exit(0);
}

void signal_ctrl_c(int signum)
{
	(void) signum;
   
	write(1, "\n", 1);
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	// rl_red
	rl_on_new_line();
	rl_redisplay();
	// signal(SIGQUIT, SIG_IGN) ??
}

void signals(void)
{
	signal(SIGINT, &signal_ctrl_c);
	signal(SIGQUIT, SIG_IGN); // ignore ctrl "\"
}
