/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:50:12 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 16:25:54 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"
// #include "include/minishell.h"
// #include <minishell.h>


void password()
{
	char	*password;

	password = readline("Enter password: ");
	if (password && ft_strcmp(password, "wachtwoord") != SUCCESS)
	{
		ft_putstr_fd("\n  , ; ,   .-''\"\"\"\'-.   , ; ,\n", 1);
		ft_putstr_fd("   \\|/  .'          '.  \\|//\n", 1);
		ft_putstr_fd("   \\-;-/   ()   ()    \\-;-/\n", 1);
		ft_putstr_fd("   // ;               ;  \\\\\n", 1);
		ft_putstr_fd("  //__; :.         .; ;___\\\\\n", 1);
		ft_putstr_fd(" `----- \'.'-.....-'.'/------'\n", 1);
		ft_putstr_fd("        '.'.-.-,_.'.'\n", 1);
		ft_putstr_fd("          '(  (..-'\n", 1);
		ft_putstr_fd("            '-' \n\n", 1);
		exit(1);
	}
	free(password);
}

void	wait_for_child(t_minishell *minishell)
{
	int status;

	t_cmds *ptr = minishell->cmds;
	while (ptr->next)
	{
		if (ptr->pid != UNSET)
			ft_waitpid(ptr->pid, &status, 0);
		ptr = ptr->next;
	}
	if (ptr->pid != UNSET)
	{
		ft_waitpid(ptr->pid, &status, 0);
		minishell->status = WEXITSTATUS(status);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	t_minishell *minishell;
	int			e_status;

	(void) argc;
	(void) argv;
	minishell = init_struct(envp);
	if (!minishell)
		return (ERROR);
	password();
	while (!minishell->exit)
	{
		signals(S_PARENT);
		minishell->line = readline("minishell$ ");
		if (!minishell->line)
			signal_ctrl_d();
		if (ft_strcmp(minishell->line, "") != SUCCESS)
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
	e_status = minishell->status;
	clear_history();
	free_mini_struct(minishell);
	return (e_status);
}