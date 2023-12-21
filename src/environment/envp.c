/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   envp.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:47 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/11 16:55:12 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include <minishell.h>
#include "../../include/minishell.h"

char	*split_value(char *envp)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i] && envp[i] != '=')
		i++;
	value = ft_substr(envp, i + 1, ft_strlen(envp));
	// alloc fail handling
	return (value);
}

char	*split_key(char *envp)
{
	char	*key;
	int		i;

	i = 0;
	while (envp[i] && envp[i] != '=')
		i++;
	key = ft_substr(envp, 0, i);
	// alloc fail handling
	return (key);
}

void	node_init(t_env **env, char *envp)
{
	t_env	*new;
	t_env	*temp;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return ; // TODO: error handling
	new->key = split_key(envp);
	new->value = split_value(envp);
	if (new->value[0] == '\0')
		new->equal_sign = false;
	else
		new->equal_sign = true;
	new->next = NULL;
	if (*env == NULL)
	{
		new->prev = NULL;
		*env = new;
	}
	else
	{
		temp = *env;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
}

t_env	*env_init(char *envp[])
{
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		node_init(&env, envp[i]);
		i++;
	}
	if (!key_exist(env, "OLDPWD"))
		node_init(&env, "OLDPWD");
	return (env);
}

t_minishell	*init_struct(char *envp[])
{
	t_minishell	*minishell;

	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL); // TODO: error handling
	minishell->env = env_init(envp);
	if (!minishell->env)
		return (NULL); // TODO: error handling
	minishell->pwd = ft_strdup(return_value(minishell->env, "PWD"));
	minishell->oldpwd = ft_strdup(return_value(minishell->env, "OLDPWD"));
	if (!minishell->pwd || !minishell->oldpwd) // free troep
		return (NULL); // TODO: error handling
	minishell->status = 0;
	minishell->exit = false; // TODO: check if this is necessary
	return (minishell);
}