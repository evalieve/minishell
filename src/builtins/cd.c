/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:28 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 15:26:52 by evalieve      ########   odam.nl         */
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
		minishell->status = E_FAILURE;
		write(STDERR_FILENO, "minishell: cd: ", 16);
		perror(cmd->args[1]); // path?!! (TODO)
	}
	// geen input cd??? else 
	// path moet nog gefreed worden nadat deze is verkregen uit return_value()... !!! (TODO)
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		minishell->status = E_FAILURE;
		write(STDERR_FILENO, "minishell: cd: ", 16);
		perror(NULL);
		// minishell->status = E_FAILURE;
		// printf("error getting cwd\n"); // TODO: error management
	}
	else
	{
		minishell->status = E_SUCCESS;
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