/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coalesce.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 20:12:30 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

void	remove_from_bin(t_arena *a, t_chunk *target)
{
	t_chunk	**head;
	int		bin;

	bin = size2bin(chunk_size(target));
	head = &a->big;
	if (bin >= 0)
		head = &a->bins[bin];
	if (target->prev)
		target->prev->next = target->next;
	else
		*head = target->next;
	if (target->next)
		target->next->prev = target->prev;
}

t_chunk	*coalesce_next(t_arena *a, t_chunk *c)
{
	t_chunk	*next;

	next = next_chunk(c);
	if (next == a->top)
		return (c);
	if ((char *)next >= (char *)a + a->total)
		return (c);
	if (!has_flag(next, CHUNK_FREE))
		return (c);
	if (has_flag(next, CHUNK_FREE) && has_flag(next, CHUNK_MMAPPED))
		return (c);
	if (has_flag(next, CHUNK_MMAPPED))
		return (c);
	remove_from_bin(a, next);
	c->size += HEADER_SIZE + chunk_size(next);
	return (c);
}

t_chunk	*coalesce_prev(t_arena *a, t_chunk *c)
{
	t_chunk	*prev;
	size_t	psize;

	if (!has_flag(c, CHUNK_PREV_FREE))
		return (c);
	psize = *(size_t *)((char *)c - sizeof(size_t));
	prev = (t_chunk *)((char *)c - sizeof(size_t) - psize - sizeof(size_t));
	if ((char *)prev < (char *)a + arena_hdr())
		return (c);
	if ((char *)prev >= (char *)c)
		return (c);
	if (!has_flag(prev, CHUNK_FREE))
		return (c);
	if (has_flag(prev, CHUNK_FREE) && has_flag(prev, CHUNK_MMAPPED))
		return (c);
	if (chunk_size(prev) != psize)
		return (c);
	if (next_chunk(prev) != c)
		return (c);
	remove_from_bin(a, prev);
	prev->size += HEADER_SIZE + chunk_size(c);
	return (prev);
}
