/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 21:38:38 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/24 13:20:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/internal.h"

void	*malloc(size_t size)
{
	t_chunk	*c;
	size_t	need;

	if (size > g_alloc.dyn_threshold)
		return (malloc_large(size));
	need = align_up(size, ALIGNMENT);
	if (need < ALIGNMENT)
		need = ALIGNMENT;
	c = tc_pop(need);
	if (c)
		return (payload_from_chunk(c));
	return (malloc_arena(need));
}
