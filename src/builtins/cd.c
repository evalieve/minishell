/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:28 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/18 15:03:53 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
// #include "../include/minishell.h"

// void	change_value_of_key(t_env *env, char *key, char *value)
// {
// 	t_env	*ptr;

// 	ptr = env;
// 	while (ptr)
// 	{
// 		if (ft_strcmp(ptr->key, key) == 0)
// 		{
// 			free(ptr->value);
// 			ptr->value = ft_strdup(value);
// 			return ;
// 		}
// 		ptr = ptr->next;
// 	}
// }

char	*return_value(t_env *env, char *key)
{
	t_env	*ptr;

	ptr = env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, key) == 0)
			return (ptr->value);
		ptr = ptr->next;
	}
	return (NULL);
}

void	builtin_cd(t_cmds *cmd, t_minishell *minishell)
{
	char	*cwd;
	char	*pwd;
	char	*oldpwd;
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
		minishell->status = 1;
		write(STDERR_FILENO, "cd: ", 4);
		perror(cmd->args[1]);
	}
	// geen input cd??? else 
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		minishell->status = 1;
		printf("error getting cwd\n"); // TODO: error management
	}
	else
	{
		minishell->status = 0;
		pwd = ft_strjoin("PWD=", cwd);
		oldpwd = ft_strjoin("OLDPWD=", minishell->pwd); // checken wanneer value pwd null is?? of niet bestaat dus 
		if (!oldpwd)
			oldpwd = ft_strdup("OLDPWD=");
		if (key_exist(minishell->env, "OLDPWD"))
			change_value(minishell->env, oldpwd);
		minishell->oldpwd = get_value(oldpwd);
		if (key_exist(minishell->env, "PWD"))
			change_value(minishell->env, pwd);
		minishell->pwd = get_value(pwd);
		free(pwd);
		free(oldpwd);
		free(cwd);
	}
}