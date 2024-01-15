/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:24:04 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/15 16:12:16 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	check_lim(t_tokens *node)
{
	if (node->prev && node->prev->type == RDHDOC)
		return (1);
	else
		return (0);
}

char	*check_var(char *line)
{
	int		i;
	char	*var;

	i = 0;
	var = NULL;
	if (ft_isalpha(line[i]) != 0 || line[i] == '_')
		i++;
	else
		return (NULL);
	while (line && (ft_isalpha(line[i]) != 0 || \
	ft_isdigit(line[i]) != 0 || line[i] == '_'))
		i++;
	var = ft_substr(line, 0, i);
	return (var);
}

char	*ft_replace(char *line, char *var, char *value, int start)
{
	char	*newline;

	newline = NULL;
	newline = ft_substr(line, 0, start);
	ft_append(&newline, value, ft_strlen(value));
	ft_append(&newline, line + start + ft_strlen(var) + 1, \
		ft_strlen(line + start + ft_strlen(var) + 1));
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
