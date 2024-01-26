/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:35 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/17 11:27:45 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	semicolumn(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ';')
			return (true);
		i++;
	}
	return (false);
}

long long	str_to_num(char *str)
{
	long long	result;
	int			changed;
	int			i;

	result = 0;
	i = 0;
	if (str[0] == '-')
		i++;
	while (str[i] && str[i] != ';')
	{
		changed = str[i] - '0';
		result = (result * 10) + changed;
		i++;
	}
	if (str[0] == '-')
		result = result * -1;
	return (result);
}

int	calc_status(int status)
{
	while (status < 0)
		status = status + 256;
	while (status > 255)
		status = status % 256;
	return (status);
}

int	convert_status(char *str)
{
	long long	status;
	int			i;

	i = 0;
	if (!str[0] || (str[0] && (str[0] == '#' || str[0] == ';')))
		return (OLD_STATUS);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i] || str[i] == ';')
		return (NON_NUMERIC);
	while (str[i] && str[i] != ';')
	{
		if (str[i] == '&' && str[i + 1] && str[i + 1] == '?')
			i++;
		else if (!(str[i] >= '0' && str[i] <= '9') || i > 11)
			return (NON_NUMERIC);
		i++;
	}
	status = str_to_num(str);
	if (status > 2147483647 || status < -2147483648)
		return (NON_NUMERIC);
	status = calc_status(status);
	return (status);
}

void	builtin_exit(t_cmds *cmd, t_minishell *minishell)
{
	int	exit_status;

	if (cmd->args[1])
	{
		exit_status = convert_status(cmd->args[1]);
		if (exit_status == NON_NUMERIC)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			error_message("exit", cmd->args[1], "numeric argument required");
			minishell->status = E_UNKNOWN;
		}
		else if (cmd->args[2] && exit_status != OLD_STATUS && \
				!semicolumn(cmd->args[1]))
		{
			error_message("exit", NULL, "too many arguments");
			minishell->status = E_FAILURE;
			return ;
		}
		else if (exit_status != OLD_STATUS)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			minishell->status = exit_status;
		}
	}
	exit(minishell->status);
}
