/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokens.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:48:32 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/31 11:04:30 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_tokens	*ft_lstnew_token(char *content, int quote)
{
	t_tokens	*new;

	new = ft_malloc(1 * sizeof(t_tokens));
	new->quote = quote;
	new->value = content;
	new->len = ft_strlen(content);
	new->prev = NULL;
	new->next = NULL;
	new->type = -1;
	new->word = -1;
	return (new);
}

t_tokens	*ft_lstlast_token(t_tokens *lst)
{
	while (lst && lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_tokens	*ft_lstfirst(t_tokens *lst)
{
	while (lst && lst->prev != NULL)
		lst = lst->prev;
	return (lst);
}

void	ft_lstremove(t_tokens *lst)
{
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	free(lst->value);
	free(lst);
}

t_tokens	*ft_lstadd(t_tokens *lst, t_tokens *new)
{
	t_tokens	*list;

	list = NULL;
	if (lst && new)
	{
		new->next = lst->next;
		new->prev = lst;
		lst->next = new;
		if (new->next)
			new->next->prev = new;
		list = new;
		return (list);
	}
	else if (lst == NULL && new)
	{
		list = new;
		return (list);
	}
	else
		return (NULL);
}
