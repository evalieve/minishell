/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: evan-der <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/06 11:18:06 by evan-der      #+#    #+#                 */
/*   Updated: 2024/01/16 18:49:40 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*last(int n, int len, char *nbr)
{
	nbr[len] = '\0';
	len--;
	nbr[0] = '0';
	if (n < 0)
	{
		nbr[0] = '-';
		if (n == -2147483648)
		{
			nbr[len] = '8';
			n = n / 10;
			len--;
		}
		n = n * -1;
	}
	while (n)
	{
		nbr[len] = (n % 10) + 48;
		n = n / 10;
		len--;
	}
	return (nbr);
}

static int	nbrlen(int n)
{
	int	len;

	len = 0;
	if (n < 0 || n == 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*nbr;
	char	*result;

	len = nbrlen(n);
	nbr = (char *)ft_malloc((len + 1) * sizeof(char));
	result = last(n, len, nbr);
	return (result);
}
