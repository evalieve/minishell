/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:57:32 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 10:55:24 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_list(t_tokens *list)
{
	t_tokens	*tmp;

	tmp = NULL;
	list = ft_lstfirst(list);
	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->value)
			free(tmp->value);
		if (tmp)
			free(tmp);
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	tmp = NULL;
	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->file)
			free(tmp->file);
		free(tmp);
	}
}

void	free_node(t_cmds *node)
{
	t_cmds	*tmp;

	tmp = NULL;
	while (node && node->prev)
		node = node->prev;
	while (node)
	{
		tmp = node;
		node = node->next;
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->args)
			free_args(tmp->args);
		if (tmp->redir)
			free_redir(tmp->redir);
		if (tmp)
			free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = NULL;
	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
