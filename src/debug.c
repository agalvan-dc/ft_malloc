/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 19:41:28 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

void	large_print_live(size_t *total)
{
	t_chunk	*c;

	pthread_mutex_lock(&g_alloc.large_lock);
	c = g_alloc.large_all;
	while (c)
	{
		putstr_safe("LARGE : ");
		puthex_safe((uintptr_t)payload_from_chunk(c));
		putstr_safe("\n");
		puthex_safe((uintptr_t)payload_from_chunk(c));
		putstr_safe(" - ");
		puthex_safe((uintptr_t)payload_from_chunk(c) + chunk_size(c));
		putstr_safe(" : ");
		putnbr_safe(chunk_size(c));
		putstr_safe(" bytes\n");
		if (total)
			*total += chunk_size(c);
		c = c->next;
	}
	pthread_mutex_unlock(&g_alloc.large_lock);
}

static void	db_line(t_chunk *c)
{
	puthex_safe((uintptr_t)payload_from_chunk(c));
	putstr_safe(" - ");
	puthex_safe((uintptr_t)payload_from_chunk(c) + chunk_size(c));
	putstr_safe(" : ");
	putnbr_safe(chunk_size(c));
	putstr_safe(" bytes [");
	if (has_flag(c, CHUNK_FREE))
		putstr_safe("F");
	if (has_flag(c, CHUNK_PREV_FREE))
		putstr_safe("P");
	if (has_flag(c, CHUNK_ZEROED))
		putstr_safe("Z");
	if (has_flag(c, CHUNK_MMAPPED))
		putstr_safe("M");
	if (has_flag(c, CHUNK_FREE) && has_flag(c, CHUNK_MMAPPED))
		putstr_safe("T");
	putstr_safe("]\n");
}

static int	db_walk(t_arena *a)
{
	t_chunk	*c;
	int		err;

	err = 0;
	c = (t_chunk *)((char *)a + arena_hdr());
	while (c < a->top)
	{
		if ((size_t)c % ALIGNMENT || chunk_size(c) % ALIGNMENT)
			err++;
		db_line(c);
		c = next_chunk(c);
	}
	return (err);
}

static void	db_arena(t_arena *a, const char *label)
{
	int	err;

	putstr_safe(label);
	putstr_safe(" arena : ");
	puthex_safe((uintptr_t)a);
	putstr_safe(" used=");
	putnbr_safe(a->used);
	putstr_safe(" total=");
	putnbr_safe(a->total);
	putstr_safe(" top=");
	puthex_safe((uintptr_t)a->top);
	putstr_safe("\n");
	err = db_walk(a);
	putstr_safe("  integrity : ");
	if (err)
		putstr_safe("FAIL\n");
	else
		putstr_safe("OK\n");
}

void	show_alloc_mem_ex(void)
{
	t_arena	*a;
	size_t	live;
	size_t	total;

	live = 0;
	total = 0;
	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		pthread_mutex_lock(&a->mutex);
		if (a->label)
			db_arena(a, "TINY ");
		else
			db_arena(a, "SMALL ");
		live += a->used;
		pthread_mutex_unlock(&a->mutex);
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	large_print_live(&total);
	putstr_safe("Total live   : ");
	putnbr_safe(live + total);
	putstr_safe(" bytes\n");
}
