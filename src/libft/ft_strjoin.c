/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:03:51 by caperale          #+#    #+#             */
/*   Updated: 2026/05/26 17:37:33 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	s = (char *)ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!s)
		return (NULL);
	while (s1[i] != '\0')
	{
		s[i] = (char)s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		s[i + j] = (char)s2[j];
		j++;
	}
	s[i + j] = '\0';
	return (s);
}
