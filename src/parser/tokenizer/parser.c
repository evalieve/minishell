/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 17:10:59 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 19:22:55 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_cmds	*parse(t_tokens *tokens)
{
	t_cmds	*list;

	list = NULL;
	while (tokens != NULL)
	{
		list = ft_nodeadd(list, ft_nodenew());
		while (tokens && tokens->type != PIPE)
		{
			if (tokens->word == CMD)
			{
				list->cmd = ft_strdup(tokens->value);
				list->args = ft_addargs(tokens);
				list->absolute = ft_isabsolute(list->cmd);
				list->builtin = ft_checkbi(list->cmd);
			}
			else if (tokens->word == FIL || tokens->word == LIM)
			{
				if (tokens->prev->type == RDOUT || \
				tokens->prev->type == RDAPPND)
					list->redir = ft_rediradd(list->redir, \
					ft_redirnew(tokens->value, tokens->prev->type));
				else if (tokens->prev->type == RDHDOC || \
				tokens->prev->type == RDIN)
					list->redir = ft_rediradd(list->redir, \
					ft_redirnew(tokens->value, tokens->prev->type));
				else
					list->redir = ft_rediradd(list->redir, \
					ft_redirnew(tokens->value, RDOUT));
			}
			tokens = tokens->next;
		}
		handle_redir(list);
		if (tokens)
			tokens = tokens->next;
	}
	while (list && list->prev)
		list = list->prev;
	return (list);
}
