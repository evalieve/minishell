/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/08 12:40:35 by evan-der      #+#    #+#                 */
/*   Updated: 2024/01/16 18:49:29 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*lst;

	lst = (t_list *)ft_malloc(sizeof(*lst));
	lst->content = content;
	lst->next = NULL;
	return (lst);
}
