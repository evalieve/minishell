/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quotes.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:57:38 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/30 18:03:33 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	closedquote(char *line, char quote)
{
	int	end;

	end = 0;
	while (line[end] != '\0' && line[end] != quote)
		end++;
	if (line[end] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (-1);
	}
	else if (line[end] == quote)
		return (end);
	return (end);
}

t_tokens	*quotes(char *line)
{
	t_tokens	*list;
	int			start;
	int			end;

	list = NULL;
	start = 0;
	end = 0;
	while (line[start] != '\0')
	{
		while (line[end] != '\0' && line[end] != '\'' && line[end] != '\"')
			end++;
		if (end != start)
			list = ft_lstadd(ft_lstlast_token(list), \
			ft_lstnew_token(ft_substr(line, start, end - start), 0));
		else if (line[end] == '\'' || line[end] == '\"')
		{
			start = end;
			end = closedquote(&line[start + 1], line[start]);
			if (end == -1)
			{
				free_list(list);
				return (NULL);
			}
			list = ft_lstadd(ft_lstlast_token(list), \
			ft_lstnew_token(ft_substr(line, start + 1, end), line[start]));
			end = end + start + 2;
		}
		start = end;
	}
	return (ft_lstlast_token(list));
}