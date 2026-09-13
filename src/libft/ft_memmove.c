/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:02:55 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 14:30:09 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	unsigned char		*sr;

	dst = (unsigned char *)dest;
	sr = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (sr < dst && dst < sr + n)
	{
		while (n--)
			dst[n] = sr[n];
	}
	else
		while (n--)
			*(dst++) = *(sr++);
	return (dest);
}
