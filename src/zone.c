/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:52:15 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/13 02:24:48 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

static const size_t g_block_sizes[] = {
	[Z_TINY] = TINY_MAX,
	[Z_SMALL] = SMALL_MAX,
	[Z_LARGE] = 0
};

static const size_t g_block_counts[] = {
	[Z_TINY] = NBLOCKS,
	[Z_SMALL] = NBLOCKS,
	[Z_LARGE] = 1 
};

t_ztype	classify(size_t size)
{
	if (size <= TINY_MAX)
		return (Z_TINY);
	if (size <= SMALL_MAX)
		return (Z_SMALL);
	return (Z_LARGE);
}

t_zone	*new_zone(size_t block_size, size_t nblocks)
{
	t_zone	*zone;
	size_t	zone_size;

	zone_size = nblocks * (sizeof(t_mhead) + block_size);
	align_up(zone_size, page_size());
	zone = (t_zone *)mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == MAP_FAILED)
		return (NULL);
	init_free_list(zone);
	g_zones = zone;
	return (zone);
}

t_zone	*find_or_create_zone(t_ztype type)
{
	size_t	block_size;
	size_t	nblocks;
	t_zone	*z;

	if (type == Z_LARGE)
		return (NULL);

	block_size = g_block_sizes[type];
	nblocks = g_block_counts[type];

	z = g_zones;
	while (z)
	{
		if (z->block_size == block_size && z->free_list != NULL)
			return (z);
		z = z->next;
	}
	return (new_zone(block_size, nblocks));
}

t_zone	*find_zone_by_ptr(void *ptr)
{

}

void	destroy_zone(t_zone *z)
{


}

