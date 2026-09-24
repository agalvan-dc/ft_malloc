/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcache.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

static __thread void	*g_tc_raw;

static t_tcache	*tc_struct(void)
{
	t_tcache	*tc;

	tc = (t_tcache *)g_tc_raw;
	if (!tc)
	{
		tc = mmap(NULL, sizeof(t_tcache), PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (tc == MAP_FAILED)
			return (NULL);
		g_tc_raw = tc;
	}
	return (tc);
}

int	tc_index(size_t payload)
{
	if (payload <= 16)
		return (0);
	if (payload > TC_MAX)
		return (-1);
	return ((int)((payload - 16) >> 4));
}

t_tcache	*tc_struct_lazy(void)
{
	return (tc_struct());
}

t_tcache	*tc_peek(void)
{
	return ((t_tcache *)g_tc_raw);
}

int	tc_push(t_arena *a, t_chunk *c)
{
	t_tcache	*tc;
	int			idx;

	(void)a;
	idx = tc_index(chunk_size(c));
	if (idx < 0)
		return (0);
	tc = tc_struct();
	if (!tc || tc->len[idx] >= tc_tray(idx))
		return (0);
	set_flag(c, CHUNK_FREE);
	set_flag(c, CHUNK_MMAPPED);
	c->next = tc->head[idx];
	tc->head[idx] = c;
	tc->len[idx]++;
	return (1);
}

int	tc_unpark(t_chunk *target)
{
	t_tcache	*tc;
	t_chunk		**link;
	int			idx;

	if (!g_tc_raw)
		return (0);
	idx = tc_index(chunk_size(target));
	if (idx < 0)
		return (0);
	tc = (t_tcache *)g_tc_raw;
	link = &tc->head[idx];
	while (*link)
	{
		if (*link == target)
		{
			*link = target->next;
			tc->len[idx]--;
			return (1);
		}
		link = &(*link)->next;
	}
	return (0);
}

t_chunk	*tc_pop(size_t need)
{
	t_tcache	*tc;
	t_chunk		*c;
	int			idx;

	if (!g_tc_raw)
		return (NULL);
	idx = tc_index(need);
	if (idx < 0)
		return (NULL);
	tc = (t_tcache *)g_tc_raw;
	if (!tc->head[idx])
		return (NULL);
	c = tc->head[idx];
	tc->head[idx] = c->next;
	tc->len[idx]--;
	c->size &= ~(CHUNK_FREE | CHUNK_MMAPPED);
	return (c);
}
