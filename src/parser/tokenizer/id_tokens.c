/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   id_tokens.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 17:19:16 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 19:17:06 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_tokens	*idtokens(t_tokens *list, t_minishell *minishell)
{
	t_tokens	*node;

	node = list;
	while (node)
	{
		if (node->quote == 1 || node->quote == 2)
			node->type = WORD;
		else if (ft_strlen(node->value) == 1)
		{
			if (node->value[0] == '|')
				node->type = PIPE;
			else if (node->value[0] == '<')
				node->type = RDIN;
			else if (node->value[0] == '>')
				node->type = RDOUT;
			else if (node->value[0] == ' ')
				node->type = WHITE;
			else
				node->type = WORD;
		}
		else if (ft_strlen(node->value) == 2)
		{
			if (ft_strcmp(node->value, ">>") == 0)
				node->type = RDAPPND;
			else if (ft_strcmp(node->value, "<<") == 0)
				node->type = RDHDOC;
			else
				node->type = WORD;
		}
		else
			node->type = WORD;
		node = node->next;
	}
	list = expand(list, minishell);
	combine_words(list);
	remove_white(list);
	list = idword(list);
	return (list);
}

t_tokens	*idword(t_tokens *tokens)
{
	t_tokens	*list;
	t_tokens	*prev;
	bool		cmd;

	cmd = false;
	list = tokens;
	prev = tokens;
	while (list)
	{
		if (list->type == PIPE)
			cmd = false;
		else if (list->type == WORD)
		{
			if (list->prev)
				prev = list->prev;
			if ((!list->prev || prev->type == PIPE || \
			prev->word == FIL) && cmd == false)
			{
				list->word = CMD;
				cmd = true;
			}
			else if (prev->word == CMD || prev->word == INPUT)
				list->word = INPUT;
			else if (prev->type == RDHDOC)
				list->word = FIL;
			else
				list->word = FIL;
		}
		prev = NULL;
		list = list->next;
	}
	return (ft_lstfirst(tokens));
}
