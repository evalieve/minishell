/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 15:03:59 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 12:50:12 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	printnode(t_cmds *node)
{
	int i;

	i = 0;
	while (node)
	{
		while (node->args && node->args[i])
		{
			printf("arg[%d]: %s\n", i, node->args[i]);
			i++;
		}
		i = 0;
		node = node->next;
	}
}

void	printlist(t_tokens *list)
{
	while (list)
	{
		if (list->value)
			printf("value: [%s]\n", list->value);
		printf("type: %d\n", list->type);
		printf("word: %d\n", list->word);
		list = list->next;
	}
}
