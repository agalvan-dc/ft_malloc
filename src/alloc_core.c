/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 12:57:35 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

void	bin_push(t_arena *a, t_chunk *c)
{
	int	bin;

	bin = size2bin(chunk_size(c));
	if (bin < 0)
		push_front(&a->big, c);
	else
	{
		push_front(&a->bins[bin], c);
		a->dirty[bin / 64] |= (unsigned long)1 << (bin % 64);
	}
}

static t_chunk	*take_chunk(t_arena *a, t_chunk *c, size_t need)
{
	t_chunk	*rest;
	t_chunk	*n;

	n = next_chunk(c);
	if ((char *)n < (char *)a + a->total
		&& !(has_flag(n, CHUNK_FREE) && has_flag(n, CHUNK_MMAPPED)))
	{
		rest = chunk_split(c, need);
		if (rest)
			bin_push(a, rest);
	}
	else
	{
		if ((char *)n < (char *)a + a->total)
			clear_flag(n, CHUNK_PREV_FREE);
	}
	clear_flag(c, CHUNK_FREE);
	clear_flag(c, CHUNK_ZEROED);
	return (c);
}

static t_chunk	*take_first(t_arena *a, t_chunk **head, size_t need)
{
	t_chunk	*curr;
	t_chunk	*prev;

	prev = NULL;
	curr = *head;
	while (curr)
	{
		if (chunk_size(curr) >= need)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			if (curr->next)
				curr->next->prev = prev;
			return (take_chunk(a, curr, need));
		}
		prev = curr;
		curr = curr->next;
	}
	return (NULL);
}

t_chunk	*find_free(t_arena *a, size_t need)
{
	t_chunk	*c;
	int		bin;
	int		i;

	bin = size2bin(need);
	if (bin >= 0)
	{
		i = bin;
		while (i < BIN_TOTAL)
		{
			if (a->dirty[i / 64] & ((unsigned long)1 << (i % 64)))
			{
				c = take_first(a, &a->bins[i], need);
				if (c)
					return (c);
				a->dirty[i / 64] &= ~((unsigned long)1 << (i % 64));
			}
			i++;
		}
	}
	c = take_first(a, &a->big, need);
	if (c)
		return (c);
	return (carve_top(a, need));
}

t_chunk	*carve_top(t_arena *a, size_t need)
{
	t_chunk	*c;
	t_chunk	*rest;
	size_t	extra;

	if ((char *)a->top >= (char *)a + a->total)
		extra = need + MIN_SPLIT + HEADER_SIZE;
	else if (chunk_size(a->top) < need + MIN_SPLIT + HEADER_SIZE)
		extra = need + MIN_SPLIT + HEADER_SIZE - chunk_size(a->top);
	else
		extra = 0;
	if (extra && !arena_extend(a, extra))
		return (NULL);
	c = a->top;
	rest = chunk_split(c, need);
	if (rest)
		a->top = rest;
	else
	{
		a->top = (t_chunk *)((char *)a + a->total);
		clear_flag(c, CHUNK_FREE);
		clear_flag(c, CHUNK_ZEROED);
	}
	set_flag(c, CHUNK_ZEROED);
	return (c);
}

t_chunk	*carve_prefetch(t_arena *a, size_t need)
{
	t_chunk	*c;
	t_chunk	*rest;
	size_t	src;
	int		idx;
	size_t	i;

	if (need > BIN_MAX_PAYLOAD)
		return (NULL);
	idx = tc_index(need);
	if (idx < 0)
		return (NULL);
	i = 0;
	while (i < tc_cap(idx))
	{
		if ((char *)a->top >= (char *)a + a->total)
			break ;
		if (chunk_size(a->top) < need + MIN_SPLIT + HEADER_SIZE)
			break ;
		c = a->top;
		src = chunk_size(c);
		rest = (t_chunk *)((char *)c + HEADER_SIZE + need);
		c->size = need | (c->size & (CHUNK_PREV_FREE | CHUNK_ZEROED));
		rest->size = (src - need - HEADER_SIZE) | CHUNK_FREE | CHUNK_ZEROED;
		rest->next = NULL;
		rest->prev = NULL;
		a->top = rest;
		if (!tc_push(a, c))
			break ;
		i++;
	}
	return (NULL);
}
