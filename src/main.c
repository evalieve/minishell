/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:50:12 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/02 14:00:27 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	empty_input(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

void	wait_for_child(t_minishell *minishell)
{
	int		status;
	t_cmds	*ptr;

	ptr = minishell->cmds;
	while (ptr->next)
	{
		if (ptr->pid != UNSET)
			ft_waitpid(ptr->pid, &status, 0);
		ptr = ptr->next;
	}
	if (ptr->pid != UNSET)
	{
		ft_waitpid(ptr->pid, &status, 0);
		if (status == SIGINT)
			minishell->status = E_SIGINT;
		else
			minishell->status = WEXITSTATUS(status);
	}
}

void	start_minishell(t_minishell *minishell)
{
	while (!minishell->exit)
	{
		signals(S_PARENT);
		minishell->line = readline("minishell$ ");
		if (!minishell->line)
			signal_ctrl_d(minishell);
		else if (!empty_input(minishell->line))
		{
			add_history(minishell->line);
			minishell->cmds = tokenize(minishell);
			if (minishell->cmds)
			{
				executor(minishell);
				wait_for_child(minishell);
			}
		}
		clean_shell(minishell);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	*minishell;
	int			e_status;

	(void) argc;
	(void) argv;
	minishell = init_mini_struct(envp);
	if (!minishell)
		return (ERROR);
	start_minishell(minishell);
	e_status = minishell->status;
	clear_history();
	free_mini_struct(minishell);
	return (e_status);
}
