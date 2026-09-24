/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 20:09:01 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

static int	free_tcache_fast(t_arena *a, t_chunk *c, size_t sz)
{
	t_tcache	*tc;
	int			idx;

	if (sz > BIN_MAX_PAYLOAD)
		return (0);
	idx = tc_index(sz);
	if (idx < 0)
		return (0);
	tc = tc_peek();
	if (!tc || tc->len[idx] >= tc_tray(idx))
		return (0);
	c->size |= CHUNK_FREE | CHUNK_MMAPPED;
	c->next = tc->head[idx];
	tc->head[idx] = c;
	tc->len[idx]++;
	a->used -= sz;
	return (1);
}

static void	free_small(t_arena *a, t_chunk *c, size_t sz)
{
	t_tcache	*tc;
	int			idx;

	a->used -= sz;
	clear_flag(c, CHUNK_ZEROED);
	idx = tc_index(sz);
	tc = (idx >= 0) ? tc_peek() : NULL;
	if (idx >= 0 && tc && tc->len[idx] < tc_cap(idx))
	{
		c = coalesce_next(a, c);
		c = coalesce_prev(a, c);
	}
	if (!tc_push(a, c))
		free_push(a, c);
	pthread_mutex_unlock(&a->mutex);
}

void	free(void *ptr)
{
	t_arena	*a;
	t_chunk	*c;
	size_t	sz;

	if (!ptr || !ptr_plausible(ptr))
		return ;
	c = chunk_from_ptr(ptr);
	sz = chunk_size(c);
	if (has_flag(c, CHUNK_FREE))
		return ;
	if (has_flag(c, CHUNK_MMAPPED))
	{
		large_free_chunk(c);
		return ;
	}
	a = g_last_arena;
	if (a && (char *)c >= (char *)a + arena_hdr()
		&& (char *)c < (char *)a + a->total)
	{
		if (free_tcache_fast(a, c, sz))
			return ;
		if (sz > TC_MAX && big_slot_park(a, c))
			return ;
	}
	a = arena_from_ptr(c);
	if (!a || has_flag(c, CHUNK_FREE))
	{
		if (a)
			pthread_mutex_unlock(&a->mutex);
		return ;
	}
	free_small(a, c, sz);
}
