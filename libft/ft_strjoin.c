/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/14 10:04:16 by evan-der      #+#    #+#                 */
/*   Updated: 2024/01/16 19:49:25 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;

	if (!s1 || !s2)
		return (NULL);
	s3 = (char *)ft_malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	ft_memcpy(s3, s1, ft_strlen(s1));
	ft_strlcpy(&s3[ft_strlen(s1)], s2, ft_strlen(s2) + 1);
	return (s3);
}
