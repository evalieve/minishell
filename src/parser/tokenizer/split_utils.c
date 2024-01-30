/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:43:26 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/29 16:05:10 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*ft_strchr_delim(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (is_delim(s[i]) == 1)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}

void	ft_append(char **line, char *buf, size_t l)
{
	char	*new_line;
	size_t	line_size;

	line_size = ft_strlen(*line);
	new_line = malloc((line_size + 1 + l) * sizeof(char));
	if (!new_line)
	{
		free(*line);
		*line = NULL;
		return ;
	}
	new_line[line_size + l] = '\0';
	while (l--)
		new_line[line_size + l] = buf[l];
	while (line_size--)
		new_line[line_size] = (*line)[line_size];
	free(*line);
	*line = new_line;
}

int	compvalue(t_tokens *list, t_tokens *next)
{
	if (list->quote == 0 && next->quote == 0)
	{
		if (ft_strlen(list->value) == 1 && ft_strlen(next->value) == 1)
		{
			if (list->value[0] == '<' || list->value[0] == '>')
				return (ft_strcmp(list->value, next->value));
		}
	}
	return (1);
}

t_tokens	*remove_white(t_tokens *head)
{
	t_tokens	*tmp;
	t_tokens	*prev;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WHITE)
		{
			if (tmp->prev)
				prev = tmp->prev;
			else if (tmp->next)
				prev = tmp->next;
			else
			{
				prev = NULL;
				head = NULL;
			}
			ft_lstremove(tmp);
			tmp = prev;
		}
		else
			tmp = tmp->next;
	}
	return (head);
}
