/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:49 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 19:20:35 by evalieve      ########   odam.nl         */
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
void	redirect(t_cmds *cmd)
{
	if (cmd->fd_in == ERROR)
		return ;
	if (cmd->fd_out == ERROR)
		return ;
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
	if (!builtin.func)
		return ;
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
	{
		if (access(cmd->cmd, X_OK) == ERROR)
		{
			free(envp);
			write(2, "minishell: ", 11);
			write(2, cmd->cmd, ft_strlen(cmd->cmd));
			write(2, ": No such file or directory\n", 29);
			exit(E_NO_SUCH_FILE_OR_DIRECTORY);
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
			exit(E_NO_SUCH_FILE_OR_DIRECTORY);
		}
		execve(path, cmd->args, envp);
	}
}

// error message bij return als pipe errort en pid errort
void	exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell)
{
	if (pipe(exec->pipe) == ERROR)
		return ;
	exec->pid = fork();
	if (exec->pid == ERROR)
		return ;
	cmds->pid = exec->pid;
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
		redirect(cmds);
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
		minishell->cmds->pid = fork();
		if (minishell->cmds->pid == ERROR)
			return ;
		if (minishell->cmds->pid == CHILD)
		{
			redirect(cmd);
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
		close(exec.pipe[READ_END]);
	}
	else
		exec_simple(tmp, minishell);
}
