/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_leaks.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/internal.h"
#include "test.h"

static size_t	used_bytes(void)
{
	t_arena	*a;
	t_chunk	*c;
	size_t	t;

	t = 0;
	pthread_mutex_lock(&g_alloc.list_lock);
	a = g_alloc.all_arenas;
	while (a)
	{
		pthread_mutex_lock(&a->mutex);
		c = (t_chunk *)((char *)a + arena_hdr());
		while (c < a->top)
		{
			if (!has_flag(c, CHUNK_FREE))
				t += chunk_size(c);
			c = next_chunk(c);
		}
		pthread_mutex_unlock(&a->mutex);
		a = a->next;
	}
	pthread_mutex_unlock(&g_alloc.list_lock);
	return (t);
}

static void	test_no_leaks(void)
{
	void	*slots[2000];
	size_t	base;
	int		i;

	base = used_bytes();
	i = 0;
	while (i < 2000)
	{
		slots[i] = malloc((size_t)(RAND() % 512) + 1);
		CHECK(slots[i] != NULL);
		i++;
	}
	CHECK(used_bytes() > base);
	i = 0;
	while (i < 2000)
	{
		free(slots[i]);
		i++;
	}
	CHECK(used_bytes() == base);
}

static void	test_leak_detected(void)
{
	void	*slots[300];
	size_t	base;
	int		i;

	base = used_bytes();
	i = 0;
	while (i < 300)
	{
		slots[i] = malloc((size_t)(RAND() % 100) + 1);
		i++;
	}
	CHECK(used_bytes() > base);
	i = 0;
	while (i < 300)
	{
		free(slots[i]);
		i++;
	}
	CHECK(used_bytes() == base);
}

int	main(void)
{
	g_rng = 4242;
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_no_leaks();
	test_leak_detected();
	show_alloc_mem();
	if (g_fail)
	{
		fprintf(stderr, "t_leaks FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_leaks PASS\n");
	return (0);
}