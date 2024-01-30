/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/25 15:25:44 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/29 13:31:09 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

bool	key_exist(t_env *env, char *arg)
{
	char	*arg_key;

	arg_key = get_key(arg);
	while (env)
	{
		if (ft_strcmp(env->key, arg_key) == SUCCESS)
		{
			free(arg_key);
			return (true);
		}
		env = env->next;
	}
	free(arg_key);
	return (false);
}

bool	validate_key(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
		return (false);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

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
