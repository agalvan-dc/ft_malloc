/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:35:08 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/13 02:09:14 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "../ft_malloc.h"

extern t_zone *g_zones;

/*    zone/zone.c   */
t_zone	*new_zone(size_t block_size, size_t nblocks);
t_zone	*find_or_create_zone(t_ztype type);
t_zone	*find_zone_by_ptr(void *ptr);
void	destroy_zone(t_zone *z);

/*    blocks/block.c   */

void	push_block(t_zone *z, t_mhead *b);
t_mhead	*pop_block(t_zone *z);
void	init_free_list(t_zone *z);

/*    utils/utils.c   */
size_t	align_up(size_t n, size_t align);
size_t	page_size(void);
void	putstr_safe(const char *s);
void	puthex_safe(uintptr_t addr);
void	putnbr_safe(size_t n);


#endif
