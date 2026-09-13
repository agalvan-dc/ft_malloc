/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:05:38 by caperale          #+#    #+#             */
/*   Updated: 2026/05/26 17:34:28 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		j;
	int		k;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = ft_strlen(s1);
	k = 0;
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (j > i && ft_strchr(set, s1[j - 1]))
		j--;
	res = ft_calloc((j - i + 1), sizeof(char));
	if (!res)
		return (NULL);
	while (i < j)
	{
		res[k] = s1[i];
		i++;
		k++;
	}
	return (res);
}
