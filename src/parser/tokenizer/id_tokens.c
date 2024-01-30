/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   id_tokens.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 17:19:16 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/30 18:19:12 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	assign_type(t_tokens *node)
{
	const static t_type	type[128] = {\
		['|'] = PIPE, \
		['<'] = RDIN, \
		['>'] = RDOUT, \
		[' '] = WHITE
	};

	while (node)
	{
		node->type = WORD;
		if (ft_strlen(node->value) == 1)
			node->type = type[(int)node->value[0]];
		else if (ft_strlen(node->value) == 2)
		{
			if (ft_strcmp(node->value, ">>") == 0)
				node->type = RDAPPND;
			else if (ft_strcmp(node->value, "<<") == 0)
				node->type = RDHDOC;
		}
		node = node->next;
	}
}

t_tokens	*idtokens(t_tokens *list, t_minishell *minishell)
{
	assign_type(list);
	list = expand(list, minishell);
	combine_words(list);
	list = remove_white(list);
	list = idword(list);
	return (list);
}

bool	idword_check(t_tokens *list, t_tokens *prev, bool cmd)
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
	return (cmd);
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
			cmd = idword_check(list, prev, cmd);
		// {
		// 	if (list->prev)
		// 		prev = list->prev;
		// 	if ((!list->prev || prev->type == PIPE || \
		// 	prev->word == FIL) && cmd == false)
		// 	{
		// 		list->word = CMD;
		// 		cmd = true;
		// 	}
		// 	else if (prev->word == CMD || prev->word == INPUT)
		// 		list->word = INPUT;
		// 	else if (prev->type == RDHDOC)
		// 		list->word = FIL;
		// 	else
		// 		list->word = FIL;
		// }
		prev = NULL;
		list = list->next;
	}
	return (ft_lstfirst(tokens));
}
