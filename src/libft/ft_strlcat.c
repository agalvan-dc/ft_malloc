/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:03:56 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 12:14:58 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_length;
	size_t	i;
	size_t	dst_length;

	src_length = ft_strlen((char *)src);
	dst_length = ft_strlen(dst);
	i = 0;
	if (size <= dst_length)
		return (size + src_length);
	while (i < (size - dst_length - 1) && src[i] != '\0')
	{
		dst[dst_length + i] = src[i];
		i++;
	}
	dst[dst_length + i] = '\0';
	return (dst_length + src_length);
}
