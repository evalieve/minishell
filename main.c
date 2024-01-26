#include "include/minishell.h"

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
}

void	wait_for_child(t_minishell *minishell)
{
	int status;

	t_cmds *ptr = minishell->cmds;
	if (ptr->pid == 0)
		return ;
	while (ptr)
	{
		waitpid(ptr->pid, &status, 0);
		ptr = ptr->next;
	}
	minishell->status = WEXITSTATUS(status);
}

int main(int argc, char *argv[], char *envp[])
{
	t_minishell *minishell;

	(void) argc;
	(void) argv;
	minishell = init_struct(envp);
	if (!minishell)
		return (ERROR);
	password();
	while (minishell->exit == false) // wanneer wordt deze op true gezet?
	{
		signals(); // signals??
		minishell->line = readline("minishell$ ");
		if (!minishell->line)
			signal_ctrl_d();
		if (ft_strcmp(minishell->line, "") != SUCCESS)
		{
			add_history(minishell->line);
			minishell->cmds = parse(minishell);
			executor(minishell);
		}
		wait_for_child(minishell);
		// printf("[MAIN] minishell->cmds->cmd: %s\n", minishell->cmds->cmd);
		// clean_shell(minishell);
	}
	// free_struct(minishell);
	return (SUCCESS);
}