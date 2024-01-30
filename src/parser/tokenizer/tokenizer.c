/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mkootstr <mkootstr@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/13 16:46:39 by mkootstr      #+#    #+#                 */
/*   Updated: 2024/01/30 16:35:47 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_tokens	*split_delim(t_tokens *last, char *new, char *rest)
{
	if (ft_strchr_delim(last->value) != NULL)
	{
		if (ft_strchr_delim(last->value) == last->value)
		{
			rest = (ft_strchr_delim(last->value) + 1);
			new = ft_substr(last->value, 0, 1);
		}
		else
		{
			rest = ft_strchr_delim(last->value);
			new = ft_substr(last->value, 0, \
			ft_strlen(last->value) - ft_strlen(rest));
		}
		if (ft_strlen(rest) > 0)
			ft_lstadd(last, ft_lstnew_token(ft_strdup(rest), 0));
		if (last->value)
			free(last->value);
		last->value = new;
	}
	return (last);
}

t_tokens	*split_input(t_tokens *old)
{
	char		*rest;
	char		*new;
	t_tokens	*last;

	last = old;
	new = NULL;
	rest = NULL;
	while (last != NULL && last->quote == 0)
	{
		last = split_delim(last, new, rest);
		last = last->next;
	}
	if (last != NULL)
		return (last);
	else
		return (ft_lstlast_token(old));
}

t_tokens	*split_tokens(t_tokens *list)
{
	while (list->prev)
			list = list->prev;
	while (1)
	{
		if (list->quote == 0)
		{
			list = split_input(list);
			if (ft_lstlast_token(list) == list)
				break ;
		}
		else if (list->next != NULL)
			list = list->next;
		else
			break ;
	}
	return (list);
}

t_cmds	*tokenize(t_minishell *minishell)
{
	t_tokens	*list;
	char		*line;

	line = minishell->line;
	list = quotes(line);
	if (list == NULL)
	{
		minishell->status = E_SYNTAX_ERROR;
		return (NULL);
	}
	list = split_tokens(list);
	list = mergetokens(ft_lstfirst(list));
	list = idtokens(list, minishell);
	if (check_syntax(list, minishell) == 1)
	{
		minishell->status = E_SYNTAX_ERROR;
		return (NULL);
	}
	minishell->cmds = parse(list);
	free_list(list);
	return (minishell->cmds);
}
