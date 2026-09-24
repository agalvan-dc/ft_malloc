/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 13:23:33 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_allocator				g_alloc = {
	.all_arenas = NULL,
	.list_lock = PTHREAD_MUTEX_INITIALIZER,
	.large_lock = PTHREAD_MUTEX_INITIALIZER,
	.dyn_threshold = DYNAMIC_THRESHOLD,
	.large_cache = {0},
	.large_all = NULL,
	.large_cached = 0,
	.main_init = 0,
	.main_thread = 0
};

size_t	arena_hdr(void)
{
	return (align_up(sizeof(t_arena), ALIGNMENT));
}

static void	*arena_map(size_t size, int use_brk)
{
	void	*base;

	base = NULL;
	if (use_brk)
	{
		base = sbrk(size);
		if (base == (void *)-1)
			base = NULL;
	}
	if (!base)
	{
		base = mmap(NULL, size, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (base == MAP_FAILED)
			base = NULL;
	}
	return (base);
}

static size_t	arena_zone_size(size_t need)
{
	size_t	size;
	size_t	base;

	base = 100 * (ZONE_TINY_MAX + HEADER_SIZE) + arena_hdr() + HEADER_SIZE;
	size = need + HEADER_SIZE + MIN_SPLIT + arena_hdr() + page_size();
	if (size < base)
		size = base;
	size = align_up(size, page_size());
	if (size < ARENA_MIN_PAGES * page_size())
		size = ARENA_MIN_PAGES * page_size();
	return (size);
}

static void	arena_init(t_arena *a, size_t size, int label, int use_brk)
{
	t_chunk	*top;

	pthread_mutex_init(&a->mutex, NULL);
	ft_bzero(a->bins, sizeof(t_chunk *) * BIN_TOTAL);
	a->dirty[0] = 0;
	a->dirty[1] = 0;
	a->big = NULL;
	a->total = size;
	a->used = 0;
	a->capacity = size;
	a->label = label;
	a->is_brk = use_brk;
	top = (t_chunk *)((char *)a + arena_hdr());
	top->size = (size - arena_hdr() - HEADER_SIZE) | CHUNK_FREE | CHUNK_ZEROED;
	top->next = NULL;
	a->top = top;
	pthread_mutex_lock(&g_alloc.list_lock);
	a->next = g_alloc.all_arenas;
	g_alloc.all_arenas = a;
	pthread_mutex_unlock(&g_alloc.list_lock);
}

t_arena	*arena_create(size_t need)
{
	void	*base;
	size_t	size;
	int		use_brk;

	if (!g_alloc.main_init)
	{
		g_alloc.main_thread = pthread_self();
		g_alloc.main_init = 1;
	}
	use_brk = (pthread_self() == g_alloc.main_thread);
	size = arena_zone_size(need);
	if (size > ARENA_MAX_PAGES * page_size())
		return (NULL);
	base = arena_map(size, use_brk);
	if (!base)
		return (NULL);
	arena_init((t_arena *)base, size, need <= ZONE_TINY_MAX, use_brk);
	return ((t_arena *)base);
}
