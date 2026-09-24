/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:02:47 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 14:30:10 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	while (n && ((unsigned long)d & 7))
	{
		*d++ = *s++;
		n--;
	}
	while (n >= 8)
	{
		*(size_t *)d = *(const size_t *)s;
		d += 8;
		s += 8;
		n -= 8;
	}
	while (n--)
		*d++ = *s++;
	return (dst);
}
