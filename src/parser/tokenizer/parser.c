/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 17:10:59 by marlou        #+#    #+#                 */
/*   Updated: 2024/02/01 14:29:57 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_cmds	*parse_loop(t_tokens *tokens, t_cmds *list)
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
		if (tokens->prev->type == RDOUT || tokens->prev->type == RDAPPND)
			list->redir = ft_rediradd(list->redir, \
			ft_redirnew(tokens->value, tokens->prev->type));
		else if (tokens->prev->type == RDHDOC || tokens->prev->type == RDIN)
			list->redir = ft_rediradd(list->redir, \
			ft_redirnew(tokens->value, tokens->prev->type));
		else
			list->redir = ft_rediradd(list->redir, \
			ft_redirnew(tokens->value, RDOUT));
	}
	return (list);
}

t_cmds	*parse(t_tokens *tokens, t_minishell *minishell)
{
	t_cmds	*list;

	list = NULL;
	while (tokens != NULL)
	{
		list = ft_nodeadd(list, ft_nodenew());
		while (tokens && tokens->type != PIPE)
		{
			list = parse_loop(tokens, list);
			tokens = tokens->next;
		}
		if (handle_redir(list, minishell) == E_SIGINT)
		{
			free_node(list);
			return (NULL);
		}
		if (tokens)
			tokens = tokens->next;
	}
	while (list && list->prev)
		list = list->prev;
	return (list);
}
