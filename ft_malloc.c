/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 21:38:38 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

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

static void	*malloc_large(size_t size)
{
	t_chunk	*c;

	if (g_alloc.dyn_threshold < THRESHOLD_MAX)
		g_alloc.dyn_threshold *= 2;
	c = large_alloc(size);
	if (!c)
		return (NULL);
	return (payload_from_chunk(c));
}

static void	*malloc_arena(size_t need)
{
	t_arena	*a;
	t_chunk	*c;

	a = get_arena(need);
	if (a)
	{
		pthread_mutex_lock(&a->mutex);
		c = find_free(a, need);
		if (c)
		{
			a->used += chunk_size(c);
			carve_prefetch(a, need);
			pthread_mutex_unlock(&a->mutex);
			return (payload_from_chunk(c));
		}
		pthread_mutex_unlock(&a->mutex);
	}
	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		pthread_mutex_lock(&a->mutex);
		c = find_free(a, need);
		if (c)
		{
			a->used += chunk_size(c);
			carve_prefetch(a, need);
			pthread_mutex_unlock(&a->mutex);
			pthread_mutex_unlock(&g_alloc.list_lock);
			g_last_arena = a;
			return (payload_from_chunk(c));
		}
		pthread_mutex_unlock(&a->mutex);
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	a = arena_create(need);
	if (!a)
		return (NULL);
	g_last_arena = a;
	pthread_mutex_lock(&a->mutex);
	c = find_free(a, need);
	if (c)
		a->used += chunk_size(c);
	pthread_mutex_unlock(&a->mutex);
	if (!c)
		return (NULL);
	return (payload_from_chunk(c));
}

void	*malloc(size_t size)
{
	t_chunk	*c;
	size_t	need;

	if (size > g_alloc.dyn_threshold)
		return (malloc_large(size));
	need = align_up(size, ALIGNMENT);
	if (need < ALIGNMENT)
		need = ALIGNMENT;
	c = tc_pop(need);
	if (c)
		return (payload_from_chunk(c));
	return (malloc_arena(need));
}
