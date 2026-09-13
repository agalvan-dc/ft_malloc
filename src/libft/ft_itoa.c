/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:01:04 by agalvan-          #+#    #+#             */
/*   Updated: 2026/06/06 15:52:21 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count(int num)
{
	size_t	i;
	long	n;

	i = 1;
	n = (long)num;
	if (n < 0)
	{
		i += 1;
		n = -n;
	}
	while ((n / 10) > 0)
	{
		n = n / 10;
		i += 1;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*cad;
	long	num;
	size_t	i;

	i = ft_count(n);
	cad = ft_calloc(i + 1, sizeof(char));
	if (!cad)
		return (NULL);
	num = (long)n;
	cad[i] = '\0';
	if (num < 0)
	{
		cad[0] = '-';
		num = -num;
	}
	while (num / 10 > 0)
	{
		cad[i - 1] = (num % 10) + '0';
		num /= 10;
		i--;
	}
	cad[i - 1] = (num % 10) + '0';
	return (cad);
}
