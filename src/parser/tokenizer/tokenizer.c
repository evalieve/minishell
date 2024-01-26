/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mkootstr <mkootstr@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/13 16:46:39 by mkootstr      #+#    #+#                 */
/*   Updated: 2024/01/26 20:02:08 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_tokens	*split_input(t_tokens *old)
{
	char		*rest;
	char		*new;
	t_tokens	*last;

	last = old;
	while (last != NULL && last->quote == 0)
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
			free(last->value);
			last->value = new;
		}
		last = last->next;
	}
	if (last != NULL)
		return (last);
	else
		return (ft_lstlast_token(old));
}

void	combine_words(t_tokens *head)
{
	t_tokens	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == WORD && temp->next && temp->next->type == WORD)
		{
			ft_append(&(temp->value), temp->next->value, temp->next->len);
			if (!temp->value)
				return ;
			temp->len += temp->next->len;
			ft_lstremove(temp->next);
		}
		else
			temp = temp->next;
	}
}

t_tokens	*mergetokens(t_tokens *list)
{
	t_tokens	*node;

	node = list;
	while (node && node->next)
	{
		if (compvalue(node, node->next) == 0)
		{
			ft_append(&(node->value), \
			node->next->value, ft_strlen(node->next->value));
			ft_lstremove(node->next);
			node = node->next;
		}
		else if (iswhspace(node->value) == 0 && node->quote == 0)
		{
			free(node->value);
			node->value = ft_strdup(" ");
			if (iswhspace(node->next->value) == 0 && node->next->quote == 0)
				ft_lstremove(node->next);
			if (node->next && iswhspace(node->next->value) != 0)
				node = node->next;
		}
		else
			node = node->next;
	}
	return (list);
}

char	**ft_addargs(t_tokens *tokens)
{
	int			i;
	char		**args;
	t_tokens	*temp;
	int			j;

	i = 0;
	args = NULL;
	temp = tokens;
	while (temp && (temp->word == INPUT || temp->word == CMD))
	{
		i++;
		temp = temp->next;
	}
	temp = tokens;
	args = malloc((i + 1) * sizeof(char *));
	args[i] = NULL;
	j = i;
	i = 0;
	while (i < j)
	{
		args[i] = ft_strdup(temp->value);
		i++;
		temp = temp->next;
	}
	return (args);
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
