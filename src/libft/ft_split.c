/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:03:28 by caperale          #+#    #+#             */
/*   Updated: 2026/05/20 10:03:29 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static int	free_all(char **split, int i)
{
	while (i >= 0)
	{
		free(split[i]);
		i--;
	}
	free(split);
	return (0);
}

static int	fill_split(char **split, char const *s, char c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[j])
	{
		while (s[j] == c)
			j++;
		start = j;
		while (s[j] && s[j] != c)
			j++;
		if (j > start)
		{
			split[i] = ft_substr(s, start, j - start);
			if (!split[i])
				return (free_all(split, i));
			i++;
		}
	}
	split[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**split;

	if (!s)
		return (NULL);
	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!fill_split(split, s, c))
		return (NULL);
	return (split);
}
