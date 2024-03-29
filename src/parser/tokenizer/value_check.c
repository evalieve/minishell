/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   value_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:46:59 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 11:05:00 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	ft_checkbi(char *command)
{
	if (ft_strcmp(command, "echo") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "cd") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "pwd") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "export") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "unset") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "env") == SUCCESS)
		return (true);
	else if (ft_strcmp(command, "exit") == SUCCESS)
		return (true);
	else
		return (false);
}

bool	ft_isabsolute(char *command)
{
	int	i;

	i = 0;
	while (command[i] != '\0')
	{
		if (command[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

bool	iswhspace(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (true);
		str++;
	}
	return (false);
}

bool	is_delim(char c)
{
	if (c == '|' || c == '>' || c == '<' || \
	c == ' ' || c == '\t' || c == '\n')
		return (true);
	else
		return (false);
}
