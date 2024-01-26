/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:36 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/26 14:51:29 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	add_to_env(t_env *env, char *arg, bool equal_sign)
{
	t_env	*ptr;
	t_env	*new;

	ptr = env;
	new = (t_env *)ft_malloc(sizeof(t_env));
	new->key = get_key(arg);
	new->equal_sign = equal_sign;
	if (!new->equal_sign)
		new->value = NULL;
	else
		new->value = get_value(arg);
	new->next = NULL;
	new->prev = NULL;
	if (!env)
		env = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
}

void	change_value(t_env *env, char *arg)
{
	t_env	*ptr;
	char	*arg_key;
	char	*arg_value;

	ptr = env;
	arg_key = get_key(arg);
	arg_value = get_value(arg);
	while (ptr)
	{
		if (ft_strcmp(ptr->key, arg_key) == SUCCESS)
		{
			free(ptr->value);
			free(arg_key);
			ptr->value = arg_value;
			if (ptr->equal_sign == false)
				ptr->equal_sign = true;
			return ;
		}
		ptr = ptr->next;
	}
}

void	print_export(t_minishell *minishell, int fd)
{
	t_env	*ptr;

	ptr = minishell->env;
	while (ptr)
	{
		print_env_line(ptr, fd);
		ptr = ptr->next;
	}
	minishell->status = E_SUCCESS;
}

bool	validate_identifier_export(char *arg, int fd)
{
	if (validate_key(arg))
		return (true);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("export: `", STDERR_FILENO);	
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}

void	builtin_export(t_cmds *cmd, t_minishell *minishell)
{
	int	i;

	i = 1;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "") == SUCCESS) // omdat er een lege string kan zijn met non existend key (expand naar "")
		i++;
	if (!cmd->args[i])
		return (print_export(minishell, cmd->fd_out));
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], "") == SUCCESS) // kan korter?
		{
			i++;
			continue ;
		}
		else if (!validate_identifier_export(cmd->args[i], cmd->fd_out))
			minishell->status = E_FAILURE;
		else if (key_exist(minishell->env, cmd->args[i]) && equal_sign_exist(cmd->args[i]))
		{
			change_value(minishell->env, cmd->args[i]);
			check_for_pwd_and_oldpwd(minishell, cmd->args[i], equal_sign_exist(cmd->args[i]));
			minishell->status = E_SUCCESS;
		}
		else if (!key_exist(minishell->env, cmd->args[i]))
		{
			add_to_env(minishell->env, cmd->args[i], equal_sign_exist(cmd->args[i]));
			check_for_pwd_and_oldpwd(minishell, cmd->args[i], equal_sign_exist(cmd->args[i]));
			minishell->status = E_SUCCESS;
		}
		i++;
	}
}

// exit status export: laatste arg geeft 0 bij success of 1 bij failure. (dus steeds aanpassen)