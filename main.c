#include "include/minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	t_minishell *minishell;

	(void) argc;
	(void) argv;
	minishell = init_struct(envp);
	if (!minishell)
		return (ERROR);
	// while (1)
	// {
		
	// 	minishell->line = readline("Enter password: ");
	// 	if (ft_strcmp(minishell->line, "wachtwoord") == SUCCESS)
	// 		break ;
	// 	if (minishell->line && ft_strcmp(minishell->line, "wachtwoord") != SUCCESS)
	// 	{
	// 		ft_putstr_fd("\n  , ; ,   .-''\"\"\"\'-.   , ; ,\n", 1);
	// 		ft_putstr_fd("   \\|/  .'          '.  \\|//\n", 1);
	// 		ft_putstr_fd("   \\-;-/   ()   ()    \\-;-/\n", 1);
	// 		ft_putstr_fd("   // ;               ;  \\\\\n", 1);
	// 		ft_putstr_fd("  //__; :.         .; ;___\\\\\n", 1);
	// 		ft_putstr_fd(" `----- \'.'-.....-'.'/------'\n", 1);
	// 		ft_putstr_fd("        '.'.-.-,_.'.'\n", 1);
	// 		ft_putstr_fd("          '(  (..-'\n", 1);
	// 		ft_putstr_fd("            '-' \n\n", 1);



	// 		exit(-1);
	// 	}
	// }
	while (minishell->exit == false)
	{
		signals(); // signals??
		minishell->line = readline("minishell$ ");
		if (!minishell->line)
			signal_ctrl_d();
		if (ft_strcmp(minishell->line, "") != SUCCESS)
		{
			add_history(minishell->line);
			minishell->cmds = parse(minishell);
			if (minishell->status == E_SYNTAX_ERROR)
				continue ;
			executor(minishell);
		}
		
		// printf("[MAIN] minishell->cmds->cmd: %s\n", minishell->cmds->cmd);
		// clean_shell(minishell);
	}
	// free_struct(minishell);
	return (SUCCESS);
}


// vvvvvvvv test main zonder tokenizer en parser vvvvvvvv

// int main(int argc, char *argv[], char *envp[])
// {
// 	t_minishell *minishell;
// 	t_cmds	**cmds;
// 	int fd1;
	
// 	minishell = init_struct(envp);

// 	// while (minishell->env)
// 	// {
// 	// 	printf("%s = %s\n", minishell->env->key, minishell->env->value);
// 	// 	minishell->env = minishell->env->next;
// 	// }

// 	fd1 = open("test.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
//     // fd2 = open("../../../../test.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);

// 	printf("argc = %d\n", argc);
// 	printf("argv[0] = %s\n", argv[0]);

// 	cmds = (t_cmds **)malloc(sizeof(t_cmds *) * 5);

// 	cmds[0] = (t_cmds *)malloc(sizeof(t_cmds));
// 	cmds[1] = (t_cmds *)malloc(sizeof(t_cmds));
// 	cmds[2] = (t_cmds *)malloc(sizeof(t_cmds));
// 	cmds[3] = (t_cmds *)malloc(sizeof(t_cmds));
//     cmds[4] = (t_cmds *)malloc(sizeof(t_cmds));

// 	cmds[0]->cmd = "unset";
//     cmds[0]->path = NULL;
//     cmds[0]->args = malloc(sizeof(char *) * 33);
//     cmds[0]->args[0] = "unset";
//     cmds[0]->args[1] = "MallocNanoZone";
//     cmds[0]->args[2] = "USER";
//     cmds[0]->args[3] = "PATH";
//     cmds[0]->args[4] = "LOGNAME";
//     cmds[0]->args[5] = "SSH_AUTH_SOCK";
//     cmds[0]->args[6] = "HOMEk";
//     cmds[0]->args[7] = "SHELL";
//     cmds[0]->args[8] = "TMPDIR";
//     cmds[0]->args[9] = "__CF_USER_TEXT_ENCODING";
//     cmds[0]->args[10] = "XPC_SERVICE_NAME";
//     cmds[0]->args[11] = "XPC_FLAGS";
//     cmds[0]->args[12] = "ORIGINAL_XDG_CURRENT_DESKTOP";
//     cmds[0]->args[13] = "SHLVL";
//     cmds[0]->args[14] = "PWD";
//     cmds[0]->args[15] = "OLDPWD";
//     cmds[0]->args[16] = "TERM_PROGRAM";
//     cmds[0]->args[17] = "TERM_PROGRAM_VERSION";
//     cmds[0]->args[18] = "LANG";
//     cmds[0]->args[19] = "COLORTERM";
//     cmds[0]->args[20] = "GIT_ASKPASS";
//     cmds[0]->args[21] = "VSCODE_GIT_ASKPASS_NODE";
//     cmds[0]->args[22] = "VSCODE_GIT_ASKPASS_EXTRA_ARGS";
//     cmds[0]->args[23] = "VSCODE_GIT_ASKPASS_MAIN";
//     cmds[0]->args[24] = "VSCODE_GIT_IPC_HANDLE";
//     cmds[0]->args[25] = "VSCODE_INJECTION";
//     cmds[0]->args[26] = "ZDOTDIR";
//     cmds[0]->args[27] = "USER_ZDOTDIR";
//     cmds[0]->args[28] = "TERM";
//     cmds[0]->args[29] = "_";
//     cmds[0]->args[30] = "__CFBundleIdentifier";
// 	cmds[0]->args[31] = "COMMAND_MODE";
// 	cmds[0]->args[32] = NULL;
//     cmds[0]->fd_in = 0;
//     cmds[0]->fd_out = fd1;
//     cmds[0]->builtin = true;
//     cmds[0]->absolute = false;
//     cmds[0]->next = NULL;
//     cmds[0]->prev = NULL;

// 	cmds[1]->cmd = "unset";
//     cmds[1]->path = NULL;
//     cmds[1]->args = malloc(sizeof(char *) * 3);
//     cmds[1]->args[0] = "unset";
// 	cmds[1]->args[1] = "PWD";
//     cmds[1]->args[2] = NULL;
//     cmds[1]->fd_in = 0;
//     cmds[1]->fd_out = 1;
//     cmds[1]->builtin = true;
//     cmds[1]->absolute = false;
//     // cmds[1]->next = cmds[2];
//     cmds[1]->next = NULL;
//     cmds[1]->prev = NULL;

// 	cmds[2]->cmd = "kip";
//     cmds[2]->path = NULL;
//     cmds[2]->args = malloc(sizeof(char *) * 3);
//     cmds[2]->args[0] = "kip";
//     cmds[2]->args[1] = "-2313";
// 	cmds[2]->args[2] = NULL;
//     cmds[2]->fd_in = 0;
//     cmds[2]->fd_out = 1;
//     cmds[2]->builtin = false;
//     cmds[2]->absolute = false;
//     // cmds[2]->next = cmds[3];
//     cmds[2]->next = NULL;
//     cmds[2]->prev = NULL;
// 	// cmds[2]->prev = cmds[1];

// 	cmds[3]->cmd = "echo";
//     cmds[3]->path = NULL;
//     cmds[3]->args = malloc(sizeof(char *) * 3);
//     cmds[3]->args[0] = "echo";
//     cmds[3]->args[1] = "exit status vanuit echo is '$?'";
// 	cmds[3]->args[2] = NULL;
//     cmds[3]->fd_in = 0;
//     cmds[3]->fd_out = 1;
//     cmds[3]->builtin = true;
//     cmds[3]->absolute = false;
//     // cmds[3]->next = cmds[3];
//     // cmds[3]->next = cmds[4];
//     // cmds[3]->prev = NULL;
// 	cmds[3]->prev = cmds[4];
// 	cmds[3]->next = NULL;

// 	cmds[4]->cmd = "exit";
//     cmds[4]->path = NULL;
//     cmds[4]->args = malloc(sizeof(char *) * 4);
//     cmds[4]->args[0] = "exit";
//     cmds[4]->args[1] = "-200;";
// 	cmds[4]->args[2] = "32;ed";
// 	cmds[4]->args[3] = NULL;
//     cmds[4]->fd_in = 0;
//     cmds[4]->fd_out = 1;
//     cmds[4]->builtin = true;
//     cmds[4]->absolute = false;
//     // cmds[4]->next = cmds[4];
//     // cmds[4]->next = NULL;
//     // cmds[4]->prev = cmds[3];
// 	cmds[4]->prev = NULL;
// 	cmds[4]->next = cmds[3];

// 	// cmds[1]->args[1] = "OLDPWD";
// 	// cmds[3]->args[1] = "..";


// 	// printf("\n\n EXPORT PRINT:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[0] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[0] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[0] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[0] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// printf("\n\n UNSET PWD:\n");
// 	// executor(cmds[1], minishell); // unset
// 	// printf("[1] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[1] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[1] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[1] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// printf("\n\n EXPORT PRINT:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[2] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[2] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[2] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[2] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// printf("\n\n CD /src/:\n");
// 	// executor(cmds[3], minishell); // cd
// 	// printf("[3] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[3] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[3] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[3] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// printf("\n\n EXPORT PRINTS:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[4] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[4] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[4] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[4] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

//     // cmds[2]->args[1] = "PWD";

// 	// printf("\n\n EXPORT PWD:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[5] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[5] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[5] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[5] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// cmds[2]->args[1] = NULL;

// 	// printf("\n\n EXPORT PRINT:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[6] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[6] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[6] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[6] env->oldpwd: %s\n\n", return_value(minishell->env, "OLDPWD"));


// 	// cmds[3]->args[1] = "..";

// 	// printf("\n\n CD ..:\n");
// 	// executor(cmds[3], minishell); // cd
// 	// printf("[7] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[7] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[7] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[7] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	// cmds[2]->args[1] = NULL;

// 	// printf("\n\n EXPORT PRINT:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[8] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[8] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[8] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[8] env->oldpwd: %s\n\n", return_value(minishell->env, "OLDPWD"));


//     // cmds[2]->args[1] = "PWD=";

// 	// printf("\n\n EXPORT PWD=:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[9] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[9] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[9] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[9] env->oldpwd: %s\n\n", return_value(minishell->env, "OLDPWD"));

// 	// cmds[2]->args[1] = NULL;

// 	// printf("\n\n EXPORT PRINT:\n");
// 	// executor(cmds[2], minishell); // export
// 	// printf("[10] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[10] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[10] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[10] env->oldpwd: %s\n\n", return_value(minishell->env, "OLDPWD"));

// 	printf("\n\nECHO - EXIT BUILTIN (PIPE):\n");
// 	executor(cmds[4], minishell); // EXIT BUILTIN (PIPE)

// 	// cmds[3]->next = NULL;
// 	printf("\n\nLS NON BUILTIN (SIMPLE):\n");
// 	executor(cmds[2], minishell); // LS NON BUILTIN (SIMPLE)
	
// 	printf("\n\nECHO BUILTIN (SIMPLE):\n");
// 	executor(cmds[3], minishell); // ECHO BUILTIN (SIMPLE)

// 	cmds[4]->next = NULL;

// 	printf("\n\nEXIT BUILTIN (SIMPLE):\n");
// 	executor(cmds[4], minishell); // EXIT BUILTIN (SIMPLE)

// 	printf("\n\nECHO BUILTIN (SIMPLE):\n");
// 	executor(cmds[3], minishell); // ECHO BUILTIN (SIMPLE)

// 	printf("\n\nECHO BUILTIN (SIMPLE):\n");
// 	executor(cmds[3], minishell); // ECHO BUILTIN (SIMPLE)

// 	// printf("\n\n ECHO BUILTIN:\n");
// 	// executor(cmds[3], minishell); // EXIT

// 	// printf("[0] minishell->pwd: %s\n", minishell->pwd);
// 	// printf("[0] env->pwd: %s\n\n", return_value(minishell->env, "PWD"));
// 	// printf("[0] minishell->oldpwd: %s\n", minishell->oldpwd);
// 	// printf("[0] env->oldpwd: %s\n", return_value(minishell->env, "OLDPWD"));

// 	return (0);
// }