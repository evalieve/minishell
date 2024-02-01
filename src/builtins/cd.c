/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:28 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/31 15:30:54 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*return_value(t_env *env, char *key)
{
	t_env	*ptr;

	ptr = env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, key) == SUCCESS)
			return (ptr->value);
		ptr = ptr->next;
	}
	return (NULL);
}

void	if_exist_change_pwds(t_minishell *minishell, char *oldpwd, char *pwd)
{
	if (key_exist(minishell->env, "OLDPWD"))
		change_value(minishell->env, oldpwd);
	if (minishell->oldpwd)
		free(minishell->oldpwd);
	minishell->oldpwd = get_value(oldpwd);
	if (key_exist(minishell->env, "PWD"))
		change_value(minishell->env, pwd);
	if (minishell->pwd)
		free(minishell->pwd);
	minishell->pwd = get_value(pwd);
}

void	handle_cwd_error(t_minishell *minishell)
{
	char	*pwd;
	char	*pwdkey;
	char	*oldpwd;

	pwd = ft_strjoin(minishell->pwd, "/..");
	if (!pwd)
		pwd = ft_strdup("/..");
	pwdkey = ft_strjoin("PWD=", pwd);
	oldpwd = ft_strjoin("OLDPWD=", minishell->pwd);
	if (!oldpwd)
		oldpwd = ft_strdup("OLDPWD=");
	if_exist_change_pwds(minishell, oldpwd, pwdkey);
	if (minishell->cwd)
		free(minishell->cwd);
	minishell->cwd = pwd;
	free(oldpwd);
	free(pwdkey);
}

void	change_pwds(t_minishell *minishell)
{
	char	*cwd;
	char	*pwd;
	char	*oldpwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(CD_E_MESSAGE, STDERR_FILENO);
		ft_putstr_fd(CWD_E_MESSAGE, STDERR_FILENO);
		perror(NULL);
		return (handle_cwd_error(minishell));
	}
	pwd = ft_strjoin("PWD=", cwd);
	oldpwd = ft_strjoin("OLDPWD=", minishell->pwd);
	if (!oldpwd)
		oldpwd = ft_strdup("OLDPWD=");
	if_exist_change_pwds(minishell, oldpwd, pwd);
	if (minishell->cwd)
		free(minishell->cwd);
	minishell->cwd = cwd;
	free(pwd);
	free(oldpwd);
}

void	builtin_cd(t_cmds *cmd, t_minishell *minishell)
{
	char	*path;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == SUCCESS)
	{
		path = return_value(minishell->env, "HOME");
		if (!path)
		{
			error_message("cd", NULL, "HOME not set");
			minishell->status = E_FAILURE;
			return ;
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) == ERROR)
	{
		minishell->status = E_FAILURE;
		non_fatal(cmd->cmd, cmd->args[1]);
		return ;
	}
	change_pwds(minishell);
	minishell->status = E_SUCCESS;
}
