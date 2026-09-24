/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 23:12:28 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

void	*calloc(size_t count, size_t size)
{
	size_t	total;
	void	*ptr;
	t_chunk	*c;

	if (size && count > SIZE_MAX / size)
		return (NULL);
	total = count * size;
	if (total == 0)
		total = 1;
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	c = chunk_from_ptr(ptr);
	if (!has_flag(c, CHUNK_ZEROED))
	{
		ft_bzero(payload_from_chunk(c), chunk_size(c));
		set_flag(c, CHUNK_ZEROED);
	}
	return (ptr);
}
