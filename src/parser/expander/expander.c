/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/22 12:05:01 by marlou        #+#    #+#                 */
/*   Updated: 2024/02/01 14:24:19 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*expand_var(char *line, t_minishell *mini, int i)
{
	char	*var;
	char	*value;

	var = ft_strdup("$");
	value = check_var(line + i + 1);
	ft_append(&var, value, ft_strlen(value));
	if (value)
		free(value);
	if (ft_strcmp(var + 1, "OLDPWD") == 0)
		value = ft_strdup(mini->oldpwd);
	else if (ft_strcmp(var + 1, "PWD") == 0)
		value = ft_strdup(mini->pwd);
	else
		value = find_var(var + 1, mini->env);
	line = ft_replace(line, var, value, i);
	if (var)
		free(var);
	return (line);
}

char	*expand_exit(char *line, t_minishell *mini, int i)
{
	char	*var;
	char	*value;

	var = ft_strdup("$?");
	value = ft_itoa(mini->status);
	line = ft_replace(line, var, value, i);
	if (var)
		free(var);
	if (value)
		free(value);
	return (line);
}

char	*ft_expand(char *line, t_minishell *mini)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != '\0' && line[i + 1] != '?')
		{
			line = expand_var(line, mini, i);
			i = 0;
		}
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line = expand_exit(line, mini, i);
			i = 0;
		}
		else
			i++;
	}
	return (line);
}

t_tokens	*expand(t_tokens *list, t_minishell *mini)
{
	t_tokens	*tokens;

	tokens = list;
	while (tokens)
	{
		if (check_lim(tokens) == 0 && \
		tokens->type == WORD && tokens->quote != 1)
			tokens->value = ft_expand(tokens->value, mini);
		tokens = tokens->next;
	}
	return (list);
}
