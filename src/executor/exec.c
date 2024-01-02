/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:49 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/21 18:27:32 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;
	
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	get_fd_out(t_cmds *cmd)
{
	if (cmd->fd_out == ERROR)
		return (ERROR);
	if (cmd->fd_out == 1)
		return (STDOUT_FILENO);
	else
		return (cmd->fd_out);
}

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
	// ptr = env;
	// while (ptr)
	// {
	// 	printf("[get_path] %s=%s\n", ptr->key, ptr->value);
	// 	ptr = ptr->next;
	// }
	ptr = env;
	// printf("[get_path] cmd: %s\n", cmd);
	while (ptr)
	{
		// printf("[get_path] %s=%s\n", ptr->key, ptr->value);
		if (ft_strcmp(ptr->key, "PATH") == SUCCESS)
		{
			// printf("[get_path] found path (!!!)\n");
			path = ft_strdup(ptr->value);
			break ;
		}
		ptr = ptr->next;
	}
	// printf("[get_path] path: %s\n", path);
	paths = ft_split(path, ':');
	// printf("[get_path] paths[%d]: %s\n", i, paths[i]);
	free(path);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		// printf("[get_path] path: %s\n", path);
		if (access(path, X_OK) == SUCCESS)
		{
			// printf("[get_path] path SUCCESS access(): %s\n", path);
			return (path); // break ; kan nu ook vgm met path = NULL iden
		}
		free(path);
		i++;
	}
	free(paths); // ?
	// printf("[get_path] path EIND: %s\n", path);
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
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL); // error
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

void	redirect(t_cmds *cmd)
{
	if (cmd->fd_in == ERROR)
		return ; // overslaan !
	if (cmd->fd_out == ERROR)
		return ; // overslaan !
	if (cmd->fd_in > 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
        cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out > 1)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
        cmd->fd_out = STDOUT_FILENO;
	}
}

static t_builtin	builtin_lookup(char *cmd)
{
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
	int	i;

	i = 0;
	while (lookup[i].cmd && ft_strcmp(lookup[i].cmd, cmd))
		i++;
	return (lookup[i]);
}

void	exec_builtin(t_cmds *cmd, t_minishell *minishell)
{
	t_builtin	builtin;

	// printf("yodiyo\n");
	builtin = builtin_lookup(cmd->cmd);
	//if (!builtin.func)
		//return ; // error message (??) weet niet eens of dit kan gebeuren
	builtin.func(cmd, minishell);
}

void	exec_command(t_cmds *cmd, t_minishell *minishell)
{
	char	*path;
	char	**envp;

	envp = env_to_envp(minishell->env);
	if (cmd->absolute) // was else if met die hierboven
	{
		if (access(cmd->cmd, X_OK) == ERROR)
		{
			free(envp);
			write(2, "minishell: ", 11);
			write(2, cmd->cmd, ft_strlen(cmd->cmd));
			write(2, ": No such file or directory\n", 29);
			exit(127); // exit code, child exit na non builtin + error message
		}
		execve(cmd->cmd, cmd->args, envp);
	}
	else
	{
		path = get_path(cmd->cmd, minishell->env);
		if (!path)
		{
			free(envp);
			write(2, "minishell: ", 11);
			write(2, cmd->cmd, ft_strlen(cmd->cmd));
			write(2, ": No such file or directory\n", 29);
			exit(127); // exit code, child exit na non builtin + error message
		}
		execve(path, cmd->args, envp);
	}
}

void	exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell)
{
	int	status;

	if (pipe(exec->pipe) == ERROR)
		return ; // error message
	exec->pid = fork();
	if (exec->pid == ERROR)
		return ; // error message
	if (exec->pid == CHILD)
	{
		if (exec->prev_read)
		{
			dup2(exec->prev_read, STDIN_FILENO);
			close(exec->prev_read);
		}
		if (cmds->next)
			dup2(exec->pipe[WRITE_END], STDOUT_FILENO);
		close(exec->pipe[READ_END]);
		close(exec->pipe[WRITE_END]);
		redirect(cmds); // , minishell) ?
		if (cmds->builtin == true)
		{
			exec_builtin(cmds, minishell);
			exit(minishell->status); // ?
		}
		else
			exec_command(cmds, minishell);
	}
	else
		waitpid(exec->pid, &status, 0);
	minishell->status = WEXITSTATUS(status);
}

void	exec_simple(t_cmds *cmd, t_minishell *minishell)
{
	int	pid;
	int	status;

	// status = 0;// ????
	// printf("cmd->cmd = %s, absolute = %d\n", cmd->cmd, cmd->absolute);
	if (cmd->builtin == true)
	{
		//ft_putstr_fd("exec_simple: builtin\n", 1);
		exec_builtin(cmd, minishell); // geen exit want geen fork
	}
	else
	{
		pid = fork();
		if (pid == ERROR)
			return ; // error message
		if (pid == CHILD)
		{
			redirect(cmd); // , minishell) ?
			exec_command(cmd, minishell);
		}
		else
			waitpid(pid, &status, 0);
		minishell->status = WEXITSTATUS(status); // want als builtin was, dan is status al set
	}
		// printf("\n[exec_simple] [parent] [%s] status: %d\n", cmd->cmd, status);
		// printf("[exec_simple] [parent] [%s] WEXITSTATUS(status): %d\n", cmd->cmd, WEXITSTATUS(status));
		// printf("[exec_simple] [parent] [%s] minishell->status: %d\n\n", cmd->cmd, minishell->status);
}

void	executor(t_minishell *minishell)
{
	t_exec exec;
	t_cmds *tmp;

	tmp = minishell->cmds;
	if (tmp->next)
	{
		while (tmp)
		{
			exec_pipe(tmp, &exec, minishell);
			if (exec.prev_read)
				close(exec.prev_read);
			exec.prev_read = exec.pipe[READ_END];
			close(exec.pipe[WRITE_END]);
			tmp = tmp->next;
		}
		// close(exec.prev_read);
		close(exec.pipe[READ_END]);
	}
	else
		exec_simple(tmp, minishell);
}