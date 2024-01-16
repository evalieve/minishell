/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/06 11:46:24 by evan-der      #+#    #+#                 */
/*   Updated: 2024/01/16 19:12:36 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ptr;

	i = 0;
	i = ft_strlen(s1);
	ptr = (char *)ft_malloc((i + 1) * sizeof(char));
	ft_strlcpy(ptr, s1, i + 1);
	return (ptr);
}
