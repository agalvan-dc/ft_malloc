/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 19:01:48 by agalvan-          #+#    #+#             */
/*   Updated: 2026/06/02 19:06:02 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_newaux(char *str)
{
	size_t		i;
	size_t		j;
	char		*c;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i] || (ft_strlen(str) == i + 1))
		return (free(str), NULL);
	++i;
	c = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!c)
		return (free(str), NULL);
	while (str[i])
		c[j++] = str[i++];
	c[j] = '\0';
	free(str);
	return (c);
}

char	*ft_nextln(char *str)
{
	char	*c;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	c = malloc(sizeof(char) * (i + 1));
	if (!c)
		return (NULL);
	while (j < i)
	{
		c[j] = str[j];
		j++;
	}
	c[j] = '\0';
	return (c);
}

char	*ft_read(int fd, char *str)
{
	ssize_t		boo;
	char		*cad;

	while (!str || !ft_strchr(str, '\n'))
	{
		cad = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!cad)
			return (free(str), NULL);
		boo = read(fd, cad, BUFFER_SIZE);
		if (boo <= -1)
			return (free(str), free(cad), NULL);
		if (boo == 0)
			return (free(cad), str);
		cad[boo] = '\0';
		if (!str)
			str = cad;
		else
		{
			str = ft_accum(cad, str);
			free(cad);
		}
	}
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*s;
	char		*c;

	if ((fd < 0 || fd >= 1024) || BUFFER_SIZE <= 0)
		return (NULL);
	s = ft_read(fd, s);
	if (!s)
		return (NULL);
	c = ft_nextln(s);
	if (!c)
		return (free(s), s = NULL, NULL);
	s = ft_newaux(s);
	return (c);
}
