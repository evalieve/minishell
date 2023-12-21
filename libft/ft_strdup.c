/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/06 11:46:24 by evan-der      #+#    #+#                 */
/*   Updated: 2023/11/13 12:24:16 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ptr;

	i = 0;
	i = ft_strlen(s1);
	ptr = (char *)malloc((i + 1) * sizeof(char));
	if (!ptr)
		return (0);
	ft_strlcpy(ptr, s1, i + 1);
	return (ptr);
}
