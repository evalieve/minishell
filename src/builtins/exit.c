/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:31:35 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/16 15:39:43 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	semicolumn(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("str[i] = %c\n", str[i]);
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
		// if (str[i] == '&' && str[i + 1] && str[i + 1] == '?')
		// 	changed = minishell->status;
		// else
			changed = str[i] - '0';
		result = (result * 10) + changed;
		i++;
	}
	if (str[0] == '-')
		result = result * -1;
	return (result);
}

int	convert_status(char *str)
{
	int	i;
	long long	status;

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
	while (status < 0)
		status = status + 256;
	while (status > 255)
		status = status % 256;
	return (status);
}

void	builtin_exit(t_cmds *cmd, t_minishell *minishell)
{
	int exit_status;

	// printf("builtin exit\n");
	if (cmd->args[1])
	{
		exit_status = convert_status(cmd->args[1]);
		// printf("\n[builtin exit] exit_status: %d\n", exit_status);

		if (exit_status == NON_NUMERIC)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			minishell->status = E_UNKNOWN;
		}
		else if (cmd->args[2] && exit_status != OLD_STATUS && !semicolumn(cmd->args[1])) // && cmd->args[1] heeft geen ; en is numeric
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			minishell->status = E_FAILURE;
			return ;
		}
		else if (exit_status != OLD_STATUS)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			minishell->status = exit_status;
		}
	}
	// printf("\n[builtin exit] exit status: %d\n", minishell->status);
	exit(minishell->status);
}

	// if args == 1 (exit)
	//		- if exit_status prev command: exit exit_status
	//		- geen exit_status: exit 0

	// if args == 2 (exit + arg)
	//		check of het numeric is
	//			- ja : atoi
	//					- te groot : error :> minishell: exit: #293297407932428484104#: numeric argument required
	//						- exit 255
	//					- 0-255: exit n
	//					- >255: %256, exit n
	//					- <255: +256, %256 exit n
	//			- nee : error :> minishell: exit: #arg#: numeric argument required
	//					- exit 255
	
	// if args > 2 (exit + arg + arg etc)
	//		als eerste arg niet numeric is: minishell: exit: #arg#: numeric argument required -> exits
	//		anders: minishell: exit: too many arguments -> exits NIET -> exit_status = 1 (cmd error)

	// in pipe: hetzelfde maar exits child en niet minishell