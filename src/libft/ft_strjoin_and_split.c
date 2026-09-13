/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_and_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 18:14:17 by agalvan-          #+#    #+#             */
/*   Updated: 2026/06/08 18:47:33 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_join_args(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!res)
		return (free(s1), NULL);
	i = 0;
	while (s1 && s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	if (s1)
		res[i++] = ' ';
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	free(s1);
	return (res);
}

char	**ft_join_and_split(int argc, char **argv)
{
	char	*mega_str;
	char	**split_matrix;
	int		i;

	mega_str = NULL;
	i = 0;
	while (i < argc)
	{
		mega_str = ft_join_args(mega_str, argv[i]);
		if (!mega_str)
			return (NULL);
		i++;
	}
	split_matrix = ft_split(mega_str, ' ');
	free(mega_str);
	return (split_matrix);
}
