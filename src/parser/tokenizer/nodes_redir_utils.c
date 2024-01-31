/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nodes_redir_utils.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:50:46 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 11:02:07 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_cmds	*ft_nodenew(void)
{
	t_cmds	*new;

	new = ft_malloc(1 * sizeof(t_cmds));
	new->cmd = NULL;
	new->fd_in = 0;
	new->fd_out = 1;
	new->args = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->builtin = false;
	new->absolute = false;
	new->redir = NULL;
	new->pid = UNSET;
	return (new);
}

t_cmds	*ft_nodeadd(t_cmds *lst, t_cmds *new)
{
	if (lst && new)
	{
		while (lst->next)
			lst = lst->next;
		new->prev = lst;
		lst->next = new;
	}
	else if (lst == NULL && new)
		lst = new;
	return (new);
}

t_redir	*ft_redirnew(char *file, t_type type)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->file = ft_strdup(file);
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_redir	*ft_rediradd(t_redir *head, t_redir *new)
{
	t_redir	*temp;

	temp = head;
	if (!temp)
		return (new);
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	return (head);
}
