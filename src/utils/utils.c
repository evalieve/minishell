/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/22 12:07:41 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 17:47:58 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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

// int	lst_len(t_cmds *cmds)
// {
// 	t_cmds	*ptr;
// 	int		len;

// 	len = 0;
// 	ptr = cmds;
// 	while (ptr)
// 	{
// 		len++;
// 		ptr = ptr->next;
// 	}
// 	return (len);
// }
