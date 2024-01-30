/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   merge_add.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 13:47:25 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/29 13:50:47 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	**alloc_args(char **args, int size)
{
	args = ft_malloc(size * sizeof(char *));
	args[size - 1] = NULL;
	return (args);
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
	args = alloc_args(args, i + 1);
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

t_tokens	*comp_merge(t_tokens *node)
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
	return (node);
}

t_tokens	*mergetokens(t_tokens *list)
{
	t_tokens	*node;

	node = list;
	while (node && node->next)
		node = comp_merge(node);
	return (list);
}
