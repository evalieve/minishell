/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:45 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 14:56:52 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
// TODO EXIT STATUS

void	delete_from_env(t_minishell *minishell, t_env *key)
{
	if (minishell->env == key)
		minishell->env = key->next;
	if (key->prev)
		key->prev->next = key->next;
	if (key->next)
		key->next->prev = key->prev;
	free(key->key);
	free(key->value);
	free(key);
}

void	key_unset(t_minishell *minishell, char *arg)
{
	t_env	*ptr;
	char	*key;

	ptr = minishell->env;
	key = get_key(arg);
	while (ptr)
	{
		if (ft_strcmp(ptr->key, key) == SUCCESS)
		{
			free(key);
			delete_from_env(minishell, ptr);
			return ;
		}
		ptr = ptr->next;
	}
	free(key);
}

void	builtin_unset(t_cmds *cmd, t_minishell *minishell)
{
	int		i;

	i = 1;
	while (cmd->args[i])
	{
		if (key_exist(minishell->env, cmd->args[i]))
			key_unset(minishell, cmd->args[i]);
		if (ft_strcmp(cmd->args[i], "PWD") == SUCCESS)
		{
			free(minishell->pwd);
			minishell->pwd = NULL;
		}
		else if (ft_strcmp(cmd->args[i], "OLDPWD") == SUCCESS)
		{
			free(minishell->oldpwd);
			minishell->oldpwd = NULL;
		}
		i++;
	}
	minishell->status = E_SUCCESS;
}
