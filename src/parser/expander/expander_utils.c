/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:24:04 by marlou        #+#    #+#                 */
/*   Updated: 2024/02/01 14:23:55 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_lim(t_tokens *node)
{
	if ((node->prev && node->prev->type == RDHDOC) || \
	(node->prev && node->prev->prev && \
	node->prev->type == WHITE && node->prev->prev->type == RDHDOC))
		return (true);
	else
		return (false);
}

char	*check_var(char *line)
{
	int		i;
	char	*var;

	i = 0;
	var = NULL;
	if (ft_isalpha(line[i]) != 0 || line[i] == '_')
		i++;
	else if (line[0] == '\0')
		return (ft_substr(line, 0, 0));
	else
		return (ft_substr(line, 0, 1));
	while (line && (ft_isalpha(line[i]) != 0 || \
	ft_isdigit(line[i]) != 0 || line[i] == '_'))
		i++;
	var = ft_substr(line, 0, i);
	return (var);
}

char	*ft_replace(char *line, char *var, char *value, int start)
{
	char	*newline;
	size_t	len;

	newline = NULL;
	len = ft_strlen(line) - ft_strlen(var) + ft_strlen(value);
	newline = ft_substr(line, 0, start);
	ft_append(&newline, value, ft_strlen(value));
	if (ft_strlen(newline) < (len))
	{
		ft_append(&newline, line + start + ft_strlen(var), \
		len - ft_strlen(newline));
	}
	free(line);
	return (newline);
}

char	*find_var(char *var, t_env *env)
{
	t_env	*list;

	list = env;
	while (list)
	{
		if (ft_strcmp(list->key, var) == 0)
			return (list->value);
		list = list->next;
	}
	return (NULL);
}
