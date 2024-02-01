/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:20:21 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/31 15:20:45 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
	envp[i] = NULL;
	i = 0;
	ptr = env;
	while (ptr)
	{
		tmp = ft_strjoin(ptr->key, "=");
		envp[i] = ft_strjoin(tmp, ptr->value);
		free(tmp);
		ptr = ptr->next;
		i++;
	}
	return (envp);
}

void	close_fds(t_cmds *cmd)
{
	if (cmd->fd_in > STDIN_FILENO)
		ft_close(cmd->fd_in);
	if (cmd->fd_out > STDOUT_FILENO)
		ft_close(cmd->fd_out);
}

char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*path;

	i = 0;
	path = NULL;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == SUCCESS)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_env *env)
{
	char	*path;
	char	**paths;

	paths = NULL;
	if (return_value(env, "PATH") != NULL)
		path = ft_strdup(return_value(env, "PATH"));
	else
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	path = find_cmd_path(paths, cmd);
	free_double_char(paths);
	return (path);
}

int	redirect(t_cmds *cmd)
{
	if (cmd->fd_in == ERROR || cmd->fd_out == ERROR)
		return (FAILURE);
	if (cmd->fd_in > STDIN_FILENO)
	{
		if (ft_dup2(cmd->fd_in, STDIN_FILENO) == ERROR)
			return (FAILURE);
		ft_close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out > STDOUT_FILENO)
	{
		if (ft_dup2(cmd->fd_out, STDOUT_FILENO) == ERROR)
			return (FAILURE);
		ft_close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
	return (SUCCESS);
}
