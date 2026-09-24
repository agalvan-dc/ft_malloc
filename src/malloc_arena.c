/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_arena.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 13:20:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/24 13:20:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

static t_arena	*get_arena(size_t need)
{
	t_arena	*a;

	a = g_last_arena;
	if (a)
		return (a);
	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	pthread_mutex_unlock(&g_alloc.list_lock);
	if (a)
	{
		g_last_arena = a;
		return (a);
	}
	return (arena_create(need));
}

void	*malloc_large(size_t size)
{
	t_chunk	*c;

	if (g_alloc.dyn_threshold < THRESHOLD_MAX)
		g_alloc.dyn_threshold *= 2;
	c = large_alloc(size);
	if (!c)
		return (NULL);
	return (payload_from_chunk(c));
}

static t_chunk	*try_take(t_arena *a, size_t need)
{
	t_chunk	*c;

	pthread_mutex_lock(&a->mutex);
	c = find_free(a, need);
	if (c)
	{
		a->used += chunk_size(c);
		carve_prefetch(a, need);
	}
	pthread_mutex_unlock(&a->mutex);
	return (c);
}

static t_chunk	*scan_arenas(size_t need)
{
	t_arena	*a;
	t_chunk	*c;

	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		c = try_take(a, need);
		if (c)
		{
			g_last_arena = a;
			pthread_mutex_unlock(&g_alloc.list_lock);
			return (c);
		}
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	return (NULL);
}

void	*malloc_arena(size_t need)
{
	t_arena	*a;
	t_chunk	*c;

	a = get_arena(need);
	if (a)
	{
		c = try_take(a, need);
		if (c)
			return (payload_from_chunk(c));
	}
	c = scan_arenas(need);
	if (c)
		return (payload_from_chunk(c));
	a = arena_create(need);
	if (!a)
		return (NULL);
	g_last_arena = a;
	c = try_take(a, need);
	if (!c)
		return (NULL);
	return (payload_from_chunk(c));
}
