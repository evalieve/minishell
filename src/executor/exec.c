/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:49 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/31 11:34:02 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_command(t_cmds *cmd, t_minishell *minishell)
{
	char	*path;
	char	**envp;

	envp = env_to_envp(minishell->env);
	if (cmd->absolute)
		path = ft_strdup(cmd->cmd);
	else
		path = get_path(cmd->cmd, minishell->env);
	if (!path && key_exist(minishell->env, "PATH"))
	{
		free_double_char(envp);
		error_message(cmd->cmd, NULL, "command not found");
		exit(E_COMMAND_NOT_FOUND);
	}
	if (access(path, X_OK) == ERROR)
	{
		free_double_char(envp);
		if (path)
			free(path);
		error_message(cmd->cmd, NULL, "No such file or directory");
		exit(E_NO_SUCH_FILE_OR_DIRECTORY);
	}
	ft_execve(path, cmd->args, envp);
}

void	exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell)
{
	signals(S_CHILD);
	ft_pipe(exec->pipe);
	cmds->pid = ft_fork();
	if (cmds->pid == CHILD)
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
			close_fds(cmds);
			exit(minishell->status);
		}
		else
			exec_command(cmds, minishell);
	}
}

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
				exit(E_FAILURE);
			exec_command(cmd, minishell);
		}
	}
	close_fds(cmd);
}

void	exec_commands_loop(t_minishell *minishell)
{
	t_exec	exec;
	t_cmds	*tmp;

	tmp = minishell->cmds;
	exec.prev_read = 0;
	if (tmp->next)
	{
		minishell->simple = false;
		while (tmp)
		{
			if (tmp->cmd)
			{
				exec_pipe(tmp, &exec, minishell);
				if (exec.prev_read)
					ft_close(exec.prev_read);
				exec.prev_read = exec.pipe[READ_END];
				ft_close(exec.pipe[WRITE_END]);
			}
			close_fds(tmp);
			tmp = tmp->next;
		}
		ft_close(exec.pipe[READ_END]);
	}
}

void	executor(t_minishell *minishell)
{
	if (minishell->cmds && minishell->cmds->next)
		exec_commands_loop(minishell);
	else if (minishell->cmds)
	{
		minishell->simple = true;
		exec_simple(minishell->cmds, minishell);
	}
}
