/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/22 12:05:01 by marlou        #+#    #+#                 */
/*   Updated: 2023/12/22 12:11:40 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *find_var(char *var, t_env *env)
{
	t_env *list;

	list = env;
	while (list)
	{
		if (ft_strcmp(list->key, var) == 0)
			return	(list->value);
		list = list->next;
	}
	return (NULL);
}

char *check_var(char *line)
{
	int i;
	char *var;

	i = 0;
	var = NULL;
	if (ft_isalpha(line[i]) != 0 || line[i] == '_')
		i++;
	else
		return (NULL);
	while (line && (ft_isalpha(line[i]) != 0 || ft_isdigit(line[i]) != 0 || line[i] == '_'))
		i++;
	var = ft_substr(line, 0, i);
	return (var);
}

char *ft_replace(char *line, char *var, char *value, int start)
{
	char *newline;

	newline = NULL;
	newline = ft_substr(line, 0, start);
	ft_append(&newline, value, ft_strlen(value));
	ft_append(&newline, line + start + ft_strlen(var), ft_strlen(line + start + ft_strlen(var)));
	free(line);
	return (newline);
}

char *ft_expand(char *line, t_env *env, t_minishell *mini)
{
	int	i;
	char *var;
	char *value;
	
	i = 0;
	var = NULL;
	value = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != '\0' && line[i + 1] != '?')
		{
			var = check_var(line + i + 1);
			value = find_var(var, env);
			line = ft_replace(line, var, value, i);
			i = 0;
		}
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			var = ft_strdup("$?");
			value = ft_itoa(mini->status);
			line = ft_replace(line, var, value, i);
			i = 0;
		}
		else i++;
	}
	return (line);
}

int	check_lim(t_tokens *node)
{
	if (node->prev && node->prev->type == RDHDOC)
		return (1);
	else
		return (0);
}

t_tokens *expand(t_tokens *list, t_minishell *mini)
{
	t_tokens *tokens;
	t_env *env;

	tokens = list;
	env = mini->env;
	while (tokens)
	{
		if (check_lim(tokens) == 0 && tokens->type == WORD && tokens->quote != 1)
			tokens->value = ft_expand(tokens->value, env, mini);
		tokens = tokens->next;
	}
	return (list);
}

