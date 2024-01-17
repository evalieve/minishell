/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:36 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/17 03:39:51 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_key(char *arg)
{
	int		i;
	char	*key;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = ft_substr(arg, 0, i);
	return (key);
}

char	*get_value(char *arg)
{
	int		i;
	char	*value;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (arg[i] == '=')
		i++;
	if (!arg[i])
		return (NULL);
	value = ft_substr(arg, i, ft_strlen(arg) - i);
	return (value);
}

bool	equal_sign_exist(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

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
			free(ptr->value); // hopelijk gaat dit nooit dubbel??
			free(arg_key);
			ptr->value = arg_value;
			if (ptr->equal_sign == false)
				ptr->equal_sign = true;
			return ;
		}
		ptr = ptr->next;
	}
}


bool	key_exist(t_env *env, char *arg)
{
	char	*arg_key;

	arg_key = get_key(arg);
	// printf("key in key_exist: %s\n", arg_key);
	while (env)
	{
		// printf("env->key = %s, arg_key = %s\n", env->key, arg_key);
		if (ft_strcmp(env->key, arg_key) == SUCCESS)
		{
			// printf(" exists\n");
			free(arg_key);
			return (true);
		}
		env = env->next;
	}
	// printf(" doesn't exists\n");
	free(arg_key);
	return (false);
}

bool	validate_key(char *arg) // key check 
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_') // key moet beginnen met letter of _
		return (false);
	while (arg[i] && arg[i] != '=') 
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	print_export(t_minishell *minishell, int fd) // dit is wel heel lelijk met print_env_line ...
{
	t_env	*ptr;

	ptr = minishell->env;
	while (ptr)
	{
		print_env_line(ptr, fd);
		ptr = ptr->next;
	}
}

void	set_working_dir(t_minishell *minishell, char *arg_key, char *arg)
{
	if (ft_strcmp(arg_key, "PWD") == SUCCESS)
	{
		if (minishell->pwd) // werkt dit wel als pwd NULL is?
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
				ptr->value = minishell->pwd;
			else
				ptr->value = minishell->oldpwd;
			ptr->equal_sign = true;
			return ;
		}
		ptr = ptr->next;
	}
}

void	check_for_pwd_and_oldpwd(t_minishell *minishell, char *arg, bool equal_sign)
{
	char	*arg_key;
	
	arg_key = get_key(arg);
	if (ft_strcmp(arg_key, "PWD") == SUCCESS || ft_strcmp(arg_key, "OLDPWD") == SUCCESS)
	{
		if (equal_sign)
			set_working_dir(minishell, arg_key, arg);
		else
			adopt_wd_value_from_struct(minishell, arg_key);
	}
	free(arg_key);
}

void	builtin_export(t_cmds *cmd, t_minishell *minishell)
{
	int	i;
	bool	equal_sign;

	i = 1;
	// printf("builtin export\n");
	minishell->status = E_SUCCESS;
	// printf("cmd->args[1]: '%s'\n", cmd->args[1]);
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "") == SUCCESS) // omdat er een lege string kan zijn
		i++;
	if (!cmd->args[i]) // export printen
		print_export(minishell, get_fd_out(cmd));
	else
	{
		while (cmd->args[i])
		{
			// get key kan niet
			if (ft_strcmp(cmd->args[i], "") == SUCCESS)
			{
				i++;
				continue ;
			}
			equal_sign = equal_sign_exist(cmd->args[i]);
			if (!validate_key(cmd->args[i])) // error function? // herloop
			{
				write(STDERR_FILENO, "export: `", 10);	
				write(STDERR_FILENO, cmd->args[i], strlen(cmd->args[i]));
				write(STDERR_FILENO, "': not a valid identifier\n", 26);
				minishell->status = E_FAILURE;
			}
			else if (key_exist(minishell->env, cmd->args[i]))
			{
				if (!equal_sign)
					return ;
				// printf("key is gevonden in environment\n");
				change_value(minishell->env, cmd->args[i]);
				check_for_pwd_and_oldpwd(minishell, cmd->args[i], equal_sign);
			}
			else // key bestaat nog niet
			{
				add_to_env(minishell->env, cmd->args[i], equal_sign);
				check_for_pwd_and_oldpwd(minishell, cmd->args[i], equal_sign);
			}
			cmd->args++;
		}
	}
}