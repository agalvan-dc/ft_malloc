/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:36:36 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/17 20:17:42 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

static void		area_unlink(t_arena *a)
{
	pthread_mutex_unlock(&g_list_lock);
	if (g_cache == a)
		g_cache = NULL;
	if (a->is_brk)
	{
			if (sbrk(0) == (char *)a + a->total)
				brk((void *)a);
	}
	else
		munmap(a, a->total);
}

t_arena			*arena_extend(t_arena *a, size_t need)
{
	size_t	grow;

	grow = align_up(need, page_size());
	if (grow < page_size())
		grow = page_size();
	if (a->is_brk)
	{
		if (sbrk(grow) == (void *)-1)
			return (NULL);
	}
	else if (mremap(a, a->total, a->total + grow, 0) == MAP_FAILED)
		return (NULL);
	a->total += grow;
	a->top->size += grow
	*(size_t *)((char *)a->top +
			HEADER_SIZE +
			(a->top->size & CHUNK_MASK) -
			sizeof(size_t)) = (a->top->size & CHUNK_MASK);
	return (a);
}

void			arena_destroy(t_arena *a)
{
	t_arena	*aux;

	aux = g_all_arenas;
	pthread_mutex_lock(&g_list_lock);
	if (aux == a)
		g_all_arenas = a->next;
	else
	{
		while (aux->next)
		{
			if (aux->next == a)
			{
				aux->next = a->next;
				break;
			}
			aux = aux->next;
		}
	}
	area_unlink(a);
}

t_arena			*find_arena_by_ptr(void *ptr)
{
	t_arena	*a;

	pthread_mutex_lock(&g_list_lock);
	a = g_all_arenas;
	while (a)
	{
		if (ptr >= (char *)a + sizeof(t_arena) && ptr < (char *)a + a->total)
		{
			pthread_mutex_lock(&a->mutex);
			pthread_mutex_unlock(&g_list_lock);
			return (a);
		}
		a = a->next;
	}
	pthread_mutex_unlock(&g_list_lock);
	return (NULL);
}

