/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:36 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 18:02:16 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
			if (ptr->value)
				free(ptr->value);
			free(arg_key);
			ptr->value = arg_value;
			if (ptr->equal_sign == false)
				ptr->equal_sign = true;
			return ;
		}
		ptr = ptr->next;
	}
	free(arg_key);
	free(arg_value);
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

bool	validate_identifier_export(char *arg)
{
	if (validate_key(arg))
		return (true);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}

void	export_variables(t_cmds *cmd, t_minishell *minishell, int i)
{
	if (!validate_identifier_export(cmd->args[i]))
		minishell->status = E_FAILURE;
	else if (key_exist(minishell->env, cmd->args[i]) && \
			equal_sign_exist(cmd->args[i]))
	{
		change_value(minishell->env, cmd->args[i]);
		check_for_pwd_and_oldpwd(minishell, cmd->args[i], \
								equal_sign_exist(cmd->args[i]));
		minishell->status = E_SUCCESS;
	}
	else if (!key_exist(minishell->env, cmd->args[i]))
	{
		add_to_env(minishell->env, cmd->args[i], \
					equal_sign_exist(cmd->args[i]));
		check_for_pwd_and_oldpwd(minishell, cmd->args[i], \
									equal_sign_exist(cmd->args[i]));
		minishell->status = E_SUCCESS;
	}
}

void	builtin_export(t_cmds *cmd, t_minishell *minishell)
{
	int	i;

	i = 1;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "") == SUCCESS)
		i++;
	if (!cmd->args[i])
		return (print_export(minishell, cmd->fd_out));
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], "") == SUCCESS)
		{
			i++;
			continue ;
		}
		export_variables(cmd, minishell, i);
		i++;
	}
}
