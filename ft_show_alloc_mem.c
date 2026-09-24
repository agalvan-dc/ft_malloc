/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show_alloc_mem.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 22:59:05 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

static void	range_line(t_chunk *c, size_t *total)
{
	uintptr_t	lo;
	uintptr_t	hi;

	lo = (uintptr_t)payload_from_chunk(c);
	hi = lo + chunk_size(c);
	puthex_safe(lo);
	putstr_safe(" - ");
	puthex_safe(hi);
	putstr_safe(" : ");
	putnbr_safe(chunk_size(c));
	putstr_safe(" bytes\n");
	*total += chunk_size(c);
}

static int	in_range(t_chunk *c, int tiny)
{
	if (tiny)
		return (chunk_size(c) <= TINY_MAX);
	return (chunk_size(c) > TINY_MAX);
}

static int	range_ok(t_chunk *c, int tiny)
{
	if (has_flag(c, CHUNK_FREE))
		return (0);
	if (chunk_size(c) == 0)
		return (0);
	if (has_flag(c, CHUNK_MMAPPED))
		return (0);
	return (in_range(c, tiny));
}

static void	print_range(t_arena *a, const char *label, int tiny,
				size_t *total)
{
	t_chunk	*c;
	int		found;

	found = 0;
	c = (t_chunk *)((char *)a + arena_hdr());
	while (c < a->top)
	{
		if (range_ok(c, tiny))
		{
			if (!found)
			{
				putstr_safe(label);
				putstr_safe(" : ");
				puthex_safe((uintptr_t)a);
				putstr_safe("\n");
				found = 1;
			}
			range_line(c, total);
		}
		c = next_chunk(c);
	}
}

void	show_alloc_mem(void)
{
	size_t	total;
	t_arena	*a;

	total = 0;
	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		pthread_mutex_lock(&a->mutex);
		print_range(a, "TINY  ", 1, &total);
		print_range(a, "SMALL ", 0, &total);
		pthread_mutex_unlock(&a->mutex);
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	large_print_live(&total);
	putstr_safe("Total : ");
	putnbr_safe(total);
	putstr_safe(" bytes\n");
}
