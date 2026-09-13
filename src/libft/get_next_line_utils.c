/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 19:02:03 by agalvan-          #+#    #+#             */
/*   Updated: 2026/06/02 19:02:42 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_accum(char *c, char *s)
{
	size_t	i;
	size_t	j;
	char	*new;

	j = 0;
	i = 0;
	new = malloc(sizeof(char) * (ft_strlen(c) + ft_strlen(s) + 1));
	if (!new)
		return (free(s), NULL);
	while (s[i])
	{
		new[i] = s[i];
		i++;
	}
	free(s);
	while (c[j])
		new[i++] = c[j++];
	new[i] = '\0';
	return (new);
}
