/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/25 15:29:07 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/02 14:12:27 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	add_to_env(t_minishell *minishell, char *arg, bool equal_sign)
{
	t_env	*ptr;
	t_env	*new;

	ptr = minishell->env;
	new = (t_env *)ft_malloc(sizeof(t_env));
	new->key = get_key(arg);
	new->equal_sign = equal_sign;
	if (!new->equal_sign)
		new->value = NULL;
	else
		new->value = get_value(arg);
	new->next = NULL;
	new->prev = NULL;
	if (!minishell->env)
		minishell->env = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
}

void	set_working_dir(t_minishell *minishell, char *arg_key, char *arg)
{
	if (ft_strcmp(arg_key, "PWD") == SUCCESS)
	{
		if (minishell->pwd)
			free(minishell->pwd);
		minishell->pwd = get_value(arg);
	}
	else
	{
		if (minishell->oldpwd)
			free(minishell->oldpwd);
		minishell->oldpwd = get_value(arg);
	}
}

void	adopt_wd_value_from_struct(t_minishell *minishell, char *arg_key)
{
	t_env	*ptr;

	ptr = minishell->env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, arg_key) == SUCCESS)
		{
			if (ft_strcmp(arg_key, "PWD") == SUCCESS)
				ptr->value = ft_strdup(minishell->pwd);
			else
				ptr->value = ft_strdup(minishell->oldpwd);
			ptr->equal_sign = true;
			return ;
		}
		ptr = ptr->next;
	}
}

void	check_for_pwd_and_oldpwd(t_minishell *minishell, \
									char *arg, bool equal_sign)
{
	char	*arg_key;

	arg_key = get_key(arg);
	if (ft_strcmp(arg_key, "PWD") == SUCCESS || \
		ft_strcmp(arg_key, "OLDPWD") == SUCCESS)
	{
		if (equal_sign)
			set_working_dir(minishell, arg_key, arg);
		else
			adopt_wd_value_from_struct(minishell, arg_key);
	}
	free(arg_key);
}
