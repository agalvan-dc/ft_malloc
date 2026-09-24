/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 12:56:10 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_chunk	*chunk_split(t_chunk *c, size_t take)
{
	t_chunk	*split;
	size_t	rest;

	if (chunk_size(c) < take + HEADER_SIZE + MIN_SPLIT)
		return (NULL);
	rest = chunk_size(c) - take - HEADER_SIZE;
	c->size = take | (c->size & (CHUNK_PREV_FREE | CHUNK_ZEROED));
	split = (t_chunk *)((char *)c + HEADER_SIZE + take);
	split->size = rest | CHUNK_FREE | CHUNK_ZEROED;
	split->next = NULL;
	split->prev = NULL;
	*(size_t *)((char *)split + rest - sizeof(size_t)) = rest;
	return (split);
}
