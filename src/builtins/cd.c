/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:28 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 19:47:16 by evalieve      ########   odam.nl         */
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

void	change_pwds(t_minishell *minishell)
{
	char	*cwd;
	char	*pwd;
	char	*oldpwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	oldpwd = ft_strjoin("OLDPWD=", minishell->pwd);
	if (!oldpwd)
		oldpwd = ft_strdup("OLDPWD=");
	if (key_exist(minishell->env, "OLDPWD"))
		change_value(minishell->env, oldpwd);
	free(minishell->oldpwd);
	minishell->oldpwd = get_value(oldpwd);
	if (key_exist(minishell->env, "PWD"))
		change_value(minishell->env, pwd);
	free(minishell->pwd);
	minishell->pwd = get_value(pwd);
	free(pwd);
	free(oldpwd);
	free(cwd);
}

void	builtin_cd(t_cmds *cmd, t_minishell *minishell)
{
	char	*path;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == SUCCESS)
	{
		path = return_value(minishell->env, "HOME");
		if (!path)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 29);
			minishell->status = E_FAILURE;
			return ;
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) == ERROR)
	{
		minishell->status = E_FAILURE;
		non_fatal("cd", cmd->args[1]);
		return ;
	}
	change_pwds(minishell);
	minishell->status = E_SUCCESS;
}
