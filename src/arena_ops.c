/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:36:36 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

__thread t_arena	*g_last_arena;

static void	area_unlink(t_arena *a)
{
	pthread_mutex_unlock(&g_alloc.list_lock);
	if (a->is_brk)
	{
		if ((char *)sbrk(0) == (char *)a + a->total)
			brk((void *)a);
	}
	else
		munmap(a, a->total);
}

t_arena	*arena_extend(t_arena *a, size_t need)
{
	size_t	grow;
	size_t	maximum;

	grow = align_up(need, page_size());
	if (grow < ARENA_MIN_PAGES * page_size())
		grow = ARENA_MIN_PAGES * page_size();
	while (grow < a->total
		&& a->total + grow < ARENA_MAX_PAGES * page_size())
		grow *= 2;
	maximum = ARENA_MAX_PAGES * page_size() - a->total;
	if (grow > maximum)
		grow = maximum;
	grow = align_up(grow, page_size());
	if (grow == 0)
		return (NULL);
	if (a->is_brk)
	{
		if (sbrk(grow) == (void *)-1)
			return (NULL);
	}
	else if (mremap(a, a->total, a->total + grow, 0) == MAP_FAILED)
		return (NULL);
	a->total += grow;
	if ((char *)a->top == (char *)a + a->total - grow)
		a->top->size = grow | CHUNK_FREE | CHUNK_ZEROED;
	else
		a->top->size += grow;
	return (a);
}

void	arena_destroy(t_arena *a)
{
	t_arena	*aux;

	pthread_mutex_lock(&g_alloc.list_lock);
	aux = g_alloc.all_arenas;
	if (aux == a)
		g_alloc.all_arenas = a->next;
	else
	{
		while (aux->next)
		{
			if (aux->next == a)
			{
				aux->next = a->next;
				break ;
			}
			aux = aux->next;
		}
	}
	area_unlink(a);
}

t_arena	*arena_from_ptr(void *ptr)
{
	t_arena	*a;

	a = g_last_arena;
	if (a && (char *)ptr >= (char *)a + arena_hdr()
		&& (char *)ptr < (char *)a + a->total)
	{
		pthread_mutex_lock(&a->mutex);
		return (a);
	}
	return (find_arena_by_ptr(ptr));
}

t_arena	*find_arena_by_ptr(void *ptr)
{
	t_arena	*a;

	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		if ((char *)ptr >= (char *)a + arena_hdr()
			&& (char *)ptr < (char *)a + a->total)
		{
			pthread_mutex_lock(&a->mutex);
			pthread_mutex_unlock(&g_alloc.list_lock);
			g_last_arena = a;
			return (a);
		}
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	return (NULL);
}
