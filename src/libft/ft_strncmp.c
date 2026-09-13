/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:05:11 by caperale          #+#    #+#             */
/*   Updated: 2026/06/08 17:34:25 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	if (n == 0)
		return (-1);
	i = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	while ((ss1[i] != '\0' || ss2[i] != '\0') && i < n)
	{
		if (ss1[i] != ss2[i])
			return ((int)(ss1[i] - ss2[i]));
		i++;
	}
	return (0);
}
