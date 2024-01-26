/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:49 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/25 15:30:42 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// int	get_fd_out(t_cmds *cmd) // waarom en waar gebruik ik deze functie allemaal? hz niet gewoon de fd_out gebruiken?
// {
// 	write(2, "GET_FD_OUT\n", 11);
// 	write(2, ft_itoa(cmd->fd_out), 1);
// 	write(2, "\n", 1);
// 	write(2, ft_itoa(STDOUT_FILENO), 1);
// 	write(2, "\n", 1);

// 	if (cmd->fd_out == ERROR) // waarom is dit logisch?
// 		return (ERROR);
// 	if (cmd->fd_out == 1) // waarom is dit logisch?
// 		return (STDOUT_FILENO);
// 	else
// 		return (cmd->fd_out); // waarom niet alleen deze?
// }

char	*get_path(char *cmd, t_env *env)
{
	char	*tmp;
	char	*path;
	char	**paths;
	t_env	*ptr;
	int		i;

	i = 0;
	paths = NULL;
	path = NULL;
	ptr = env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "PATH") == SUCCESS)
		{
			path = ft_strdup(ptr->value);
			break ;
		}
		ptr = ptr->next;
	}
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == SUCCESS)
			return (path); // break ; kan nu ook vgm met path = NULL iden
		free(path);
		i++;
	}
	free(paths); // ?
	return (NULL);
}

char	**env_to_envp(t_env *env)
{
	t_env	*ptr;
	char	**envp;
	char	*tmp;
	int		i;

	i = 0;
	ptr = env;
	while (ptr)
	{
		i++;
		ptr = ptr->next;
	}
	envp = (char **)ft_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		envp[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// errors checken
int	redirect(t_cmds *cmd)
{
	// printf("REDIRECT:cmd=%s fd = %d && fd_out = %d\n",cmd->cmd, cmd->fd_out, STDOUT_FILENO);
	if (cmd->fd_in == ERROR || cmd->fd_out == ERROR)
		return (FAILURE);
	if (cmd->fd_in > STDIN_FILENO)
	{
		if (ft_dup2(cmd->fd_in, STDIN_FILENO) == ERROR)
			return (FAILURE);
		ft_close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO; // WEL NODIG MET PIPE en builtin???
	}
	if (cmd->fd_out > STDOUT_FILENO)
	{
		// printf("REDIRECT fd = %d && fd_out = %d\n", cmd->fd_out, STDOUT_FILENO);
		// int fd2 = cmd->fd_out;
		if (ft_dup2(cmd->fd_out, STDOUT_FILENO) == ERROR)
			return (FAILURE);
		ft_close(cmd->fd_out);
		// printf("REDIRECT fd = %d && fd_out = %d && fd2 = %d\n", cmd->fd_out, STDOUT_FILENO, fd2);
		cmd->fd_out = STDOUT_FILENO; // dit slaat nergens op want ik gebruik de fd_out niet meer >>> WEL>?? 
	}
	return (SUCCESS);
}

static t_builtin	builtin_lookup(char *cmd)
{
	int						i;
	static const t_builtin	lookup[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"exit", builtin_exit},
	{NULL, NULL}
	};

	i = 0;
	while (lookup[i].cmd && ft_strcmp(lookup[i].cmd, cmd))
		i++;
	return (lookup[i]);
}

// error message (??) weet niet eens of dit kan gebeuren [BIJ RETURN ;]
void	exec_builtin(t_cmds *cmd, t_minishell *minishell)
{
	t_builtin	builtin;

	builtin = builtin_lookup(cmd->cmd);
	// if (!builtin.func)
	// 	return ;
	builtin.func(cmd, minishell);
}

// errors checken en kijken of hier geen functie voor is
// exit code, child exit na non builtin + error message bij beide errors
void	exec_command(t_cmds *cmd, t_minishell *minishell)
{
	char	*path;
	char	**envp;

	envp = env_to_envp(minishell->env);
	if (cmd->absolute)
		path = ft_strdup(cmd->cmd);
	else
		path = get_path(cmd->cmd, minishell->env);
	if (!path)
	{
		free(envp);
		error_message(cmd->cmd, NULL, "command not found");
		exit(E_COMMAND_NOT_FOUND);
	}
	if (access(path, X_OK) == ERROR)
	{
		free(envp);
		error_message(cmd->cmd, NULL, "No such file or directory");
		exit(E_NO_SUCH_FILE_OR_DIRECTORY);
	}
	ft_execve(path, cmd->args, envp);
}

void	exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell)
{
	signals(S_CHILD);
	ft_pipe(exec->pipe);
	exec->pid = ft_fork();
	cmds->pid = exec->pid;
	if (exec->pid == CHILD)
	{
		if (exec->prev_read)
		{
			ft_dup2(exec->prev_read, STDIN_FILENO);
			ft_close(exec->prev_read);
		}
		if (cmds->next)
			ft_dup2(exec->pipe[WRITE_END], STDOUT_FILENO);
		ft_close(exec->pipe[READ_END]);
		ft_close(exec->pipe[WRITE_END]);
		if (redirect(cmds) == FAILURE)
			exit(E_FAILURE);
		if (cmds->builtin)
		{
			exec_builtin(cmds, minishell);
			exit(minishell->status);
		}
		else
			exec_command(cmds, minishell);
	}
}

// error message bij return als pid errort
void	exec_simple(t_cmds *cmd, t_minishell *minishell)
{
	if (cmd->builtin)
		exec_builtin(cmd, minishell);
	else
	{
		signals(S_CHILD);
		minishell->cmds->pid = ft_fork();
		if (minishell->cmds->pid == CHILD)
		{
			if (redirect(cmd) == FAILURE)
				exit(E_FAILURE); // ??? 127
			exec_command(cmd, minishell);
		}
	}
}

int	lst_len(t_cmds *cmds)
{
	int	len;

	len = 0;
	while (cmds)
	{
		len++;
		cmds = cmds->next;
	}
	return (len);
}

void	executor(t_minishell *minishell)
{
	t_exec	exec;
	t_cmds	*tmp;

	tmp = minishell->cmds;
	exec.prev_read = 0;
	if (tmp->next)
	{
		while (tmp)
		{
			exec_pipe(tmp, &exec, minishell);
			if (exec.prev_read)
				ft_close(exec.prev_read);
			exec.prev_read = exec.pipe[READ_END];
			ft_close(exec.pipe[WRITE_END]);
			tmp = tmp->next;
		}
		ft_close(exec.pipe[READ_END]);
	}
	else
		exec_simple(tmp, minishell);
}
