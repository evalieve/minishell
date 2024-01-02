/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   syntax_check.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/28 17:31:30 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/02 15:51:39 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fatal(char *str, t_minishell *mini)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putchar_fd('\n', 2);
	free_all(mini);
	exit(mini->status);
}

void	free_list(t_tokens *list)
{
	t_tokens *tmp;

	tmp = NULL;
	list = ft_lstfirst(list);
	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redir(t_redir *redir)
{
	t_redir *tmp;

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
	t_cmds *tmp;

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
		if (tmp->in)
			free_redir(tmp->in);
		if (tmp->out)	
			free_redir(tmp->out);
		free(tmp);
	//fd closen???
	}
}

void	free_env(t_env *env)
{
	t_env *tmp;

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

void	check_syntax(t_tokens *tokens, t_minishell *mini)
{
	t_tokens *tmp;

	tmp = tokens;
	while (tmp && tmp->next)
	{
		if (tmp->type == PIPE && tmp->next->type == PIPE)
		{
			free_list(tokens);
			fatal("syntax error near unexpected token `||'", mini);
		}
	}
}

