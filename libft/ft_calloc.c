/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/29 11:54:44 by evan-der      #+#    #+#                 */
/*   Updated: 2024/01/16 22:26:20 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = (void *)ft_malloc(size * count);
	ft_bzero(ptr, size * count);
	return (ptr);
}
