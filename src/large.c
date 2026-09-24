/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   large.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

#define LARGE_PRE 16
#define LARGE_MAGIC 0x4D414C4C4F434244ULL

static int	large_bucket(size_t len)
{
	int		i;
	size_t	bound;

	i = 0;
	bound = page_size() * 2;
	while (i < NUM_LARGE_BUCKETS - 1 && len > bound)
	{
		bound *= 2;
		i++;
	}
	return (i);
}

static t_chunk	*large_mmap(size_t size)
{
	size_t	len;
	size_t	need;
	t_chunk	*c;
	void	*base;

	if (size > SIZE_MAX - LARGE_PRE - HEADER_SIZE)
		return (NULL);
	len = align_up(size + LARGE_PRE + HEADER_SIZE, page_size());
	base = mmap(NULL, len, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (base == MAP_FAILED)
		return (NULL);
	*(size_t *)base = len;
	*(size_t *)((char *)base + 8) = LARGE_MAGIC;
	c = (t_chunk *)((char *)base + LARGE_PRE);
	need = align_up(size, ALIGNMENT);
	c->size = need | CHUNK_MMAPPED | CHUNK_ZEROED;
	pthread_mutex_lock(&g_alloc.large_lock);
	c->next = g_alloc.large_all;
	g_alloc.large_all = c;
	pthread_mutex_unlock(&g_alloc.large_lock);
	return (c);
}

t_chunk	*large_alloc(size_t size)
{
	t_chunk	**link;
	t_chunk	*c;

	if (size > SIZE_MAX - LARGE_PRE - HEADER_SIZE)
		return (NULL);
	link = &g_alloc.large_cache[large_bucket(align_up(size, ALIGNMENT))];
	pthread_mutex_lock(&g_alloc.large_lock);
	while (*link && chunk_size(*link) < align_up(size, ALIGNMENT))
		link = &(*link)->next;
	c = *link;
	if (c)
	{
		*link = c->next;
		g_alloc.large_cached -= *(size_t *)((char *)c - LARGE_PRE);
		c->next = g_alloc.large_all;
		g_alloc.large_all = c;
	}
	pthread_mutex_unlock(&g_alloc.large_lock);
	if (!c)
		return (large_mmap(size));
	return (c);
}

void	large_free_chunk(t_chunk *c)
{
	t_chunk	**pp;
	size_t	len;

	if (*(size_t *)((char *)c - LARGE_PRE + 8) != LARGE_MAGIC)
		return ;
	len = *(size_t *)((char *)c - LARGE_PRE);
	pthread_mutex_lock(&g_alloc.large_lock);
	pp = &g_alloc.large_all;
	while (*pp && *pp != c)
		pp = &(*pp)->next;
	if (*pp)
		*pp = c->next;
	if (len + g_alloc.large_cached <= LARGE_CACHE_CAP)
	{
		c->next = g_alloc.large_cache[large_bucket(len)];
		g_alloc.large_cache[large_bucket(len)] = c;
		g_alloc.large_cached += len;
	}
	else
		munmap((char *)c - LARGE_PRE, len);
	pthread_mutex_unlock(&g_alloc.large_lock);
}

void	*large_realloc(t_chunk *c, size_t need)
{
	t_chunk	*nc;
	void	*new_ptr;
	size_t	old;

	old = chunk_size(c);
	if (need <= old)
		return (payload_from_chunk(c));
	nc = large_alloc(need);
	if (!nc)
		return (NULL);
	new_ptr = payload_from_chunk(nc);
	ft_memcpy(new_ptr, payload_from_chunk(c), old);
	large_free_chunk(c);
	return (new_ptr);
}
