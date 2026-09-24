/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

void	free_push(t_arena *a, t_chunk *c)
{
	t_chunk	*n;
	size_t	top;

	n = next_chunk(c);
	if ((char *)n < (char *)a + a->total && n == a->top)
	{
		top = chunk_size(n);
		if (n == a->top)
		{
			a->top = c;
			c->size = (chunk_size(c) + HEADER_SIZE + top)
				| CHUNK_FREE | CHUNK_ZEROED;
			clear_flag(c, CHUNK_PREV_FREE);
			return;
		}
	}
	if ((char *)n < (char *)a + a->total)
		set_flag(n, CHUNK_PREV_FREE);
	*(size_t *)((char *)c + chunk_size(c) - sizeof(size_t)) = chunk_size(c);
	set_flag(c, CHUNK_FREE);
	bin_push(a, c);
}
