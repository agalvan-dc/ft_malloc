/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 21:15:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

static void	*realloc_copy(t_arena *a, t_chunk *c, size_t need, void *ptr)
{
	void	*new_ptr;

	pthread_mutex_unlock(&a->mutex);
	new_ptr = malloc(need);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, chunk_size(c));
	free(ptr);
	return (new_ptr);
}

static int	grow_neighbor(t_arena *a, t_chunk *c, size_t need)
{
	t_chunk	*n;
	t_chunk	*grown;

	n = next_chunk(c);
	if ((char *)n >= (char *)a + a->total)
		return (0);
	if (has_flag(n, CHUNK_FREE) && !has_flag(n, CHUNK_MMAPPED))
		coalesce_next(a, c);
	else if (n != a->top)
		return (0);
	if (chunk_size(c) < need)
	{
		if (n != a->top)
			return (0);
		grown = carve_top(a, need - chunk_size(c) - HEADER_SIZE);
		if (!grown)
			return (0);
		c->size += HEADER_SIZE + chunk_size(grown);
	}
	return (1);
}

static int	grow_in_place(t_arena *a, t_chunk *c, size_t need, size_t old)
{
	t_chunk	*n;
	int		ret;

	n = next_chunk(c);
	while (chunk_size(c) < need && (char *)n < (char *)a + a->total
		&& has_flag(n, CHUNK_FREE) && has_flag(n, CHUNK_MMAPPED)
		&& tc_unpark(n))
	{
		c->size += HEADER_SIZE + chunk_size(n);
		n = next_chunk(c);
	}
	if (chunk_size(c) < need)
		n = next_chunk(c);
	if (chunk_size(c) < need && !grow_neighbor(a, c, need))
		ret = 0;
	else
		ret = 1;
	if (!ret)
		return (0);
	a->used += chunk_size(c) - old;
	return (1);
}

static void	*realloc_arena(void *ptr, size_t need)
{
	t_arena	*a;
	t_chunk	*c;
	t_chunk	*rest;
	size_t	old;

	c = chunk_from_ptr(ptr);
	a = arena_from_ptr(c);
	if (!a)
		return (NULL);
	old = chunk_size(c);
	if (old >= need)
	{
		rest = chunk_split(c, need);
		if (rest)
		{
			a->used -= chunk_size(rest);
			free_push(a, rest);
		}
		pthread_mutex_unlock(&a->mutex);
		return (ptr);
	}
	if (!grow_in_place(a, c, need, old))
		return (realloc_copy(a, c, need, ptr));
	pthread_mutex_unlock(&a->mutex);
	return (ptr);
}

void	*realloc(void *ptr, size_t size)
{
	t_chunk	*c;
	size_t	need;

	if (!ptr)
		return (malloc(size));
	if (!size)
	{
		free(ptr);
		return (NULL);
	}
	need = align_up(size, ALIGNMENT);
	if (need < ALIGNMENT)
		need = ALIGNMENT;
	if (!ptr_plausible(ptr))
		return (NULL);
	c = chunk_from_ptr(ptr);
	if (has_flag(c, CHUNK_MMAPPED))
		return (large_realloc(c, need));
	return (realloc_arena(ptr, need));
}
