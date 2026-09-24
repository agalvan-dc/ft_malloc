/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcache.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

__thread void			*g_tc_raw;
static __thread t_chunk	*g_big_slot;
static __thread t_arena	*g_big_arena;

static t_tcache	*tc_struct(void)
{
	t_tcache	*tc;

	tc = (t_tcache *)g_tc_raw;
	if (!tc)
	{
		tc = mmap(NULL, sizeof(t_tcache), PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (tc == MAP_FAILED)
			return (NULL);
		g_tc_raw = tc;
	}
	return (tc);
}

int	tc_push(t_arena *a, t_chunk *c)
{
	t_tcache	*tc;
	int			idx;

	(void)a;
	idx = tc_index(chunk_size(c));
	if (idx < 0)
		return (0);
	tc = tc_struct();
	if (!tc || tc->len[idx] >= tc_tray(idx))
		return (0);
	c->size |= CHUNK_FREE | CHUNK_MMAPPED;
	c->next = tc->head[idx];
	tc->head[idx] = c;
	tc->len[idx]++;
	return (1);
}

int	tc_unpark(t_chunk *target)
{
	t_tcache	*tc;
	t_chunk		**link;
	int			idx;

	if (!g_tc_raw)
		return (0);
	idx = tc_index(chunk_size(target));
	if (idx < 0)
		return (0);
	tc = (t_tcache *)g_tc_raw;
	link = &tc->head[idx];
	while (*link)
	{
		if (*link == target)
		{
			*link = target->next;
			tc->len[idx]--;
			return (1);
		}
		link = &(*link)->next;
	}
	return (0);
}

int	big_slot_park(t_arena *a, t_chunk *c)
{
	if (g_big_slot)
		return (0);
	g_big_slot = c;
	g_big_arena = a;
	c->size |= CHUNK_FREE | CHUNK_MMAPPED;
	clear_flag(c, CHUNK_ZEROED);
	a->used -= chunk_size(c);
	return (1);
}

t_chunk	*big_slot_pop(size_t need)
{
	t_chunk	*c;
	t_arena	*a;
	size_t	sz;

	c = g_big_slot;
	if (!c)
		return (NULL);
	sz = chunk_size(c);
	a = g_big_arena;
	if (sz != need)
	{
		g_big_slot = NULL;
		g_big_arena = NULL;
		c->size &= ~CHUNK_MMAPPED;
		pthread_mutex_lock(&a->mutex);
		free_push(a, c);
		pthread_mutex_unlock(&a->mutex);
		return (NULL);
	}
	g_big_slot = NULL;
	g_big_arena = NULL;
	if (a)
		a->used += sz;
	c->size &= ~(CHUNK_FREE | CHUNK_MMAPPED | CHUNK_ZEROED);
	return (c);
}
