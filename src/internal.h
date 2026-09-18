/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 internal.h											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: agalvan- <agalvan-@student.42madrid.c		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/09/13 00:35:08 by agalvan-		   #+#	  #+#			  */
/*	 Updated: 2026/09/15 23:58:50 by agalvan-		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "../ft_malloc.h"

extern __thread t_arena		*g_cache;
extern	t_arena			*g_all_arenas;
extern	pthread_mutex_t		g_list_lock;
extern	pthread_mutex_t		g_large_lock;
extern	t_chunk			*g_large_cache[NUM_LARGE_BUCKETS];
extern	size_t			g_dyn_threshold;


static inline size_t	align_up(size_t n, size_t align)
{
	return ((n + (align - 1)) & ~(align - 1));
}
static inline t_chunk	*chunk_from_ptr(void *ptr)
{
	return ((t_chunk *)((char *)ptr - HEADER_SIZE));
}
static inline void	*payload_from_chunk(t_chunk *c)
{
	return ((void *)((char *)c + HEADER_SIZE));
}
static inline t_chunk	*next_chunk(t_chunk *c)
{
	return ((t_chunk *)((char *)c + HEADER_SIZE + (c->size & CHUNK_MASK)));
}
static inline size_t	chunk_size(t_chunk *c)
{
	return (c->size & CHUNK_MASK);
}
static inline int	has_flag(const t_chunk *c, size_t f)
{
	return ((c->size & f) != 0);
}
static inline void	set_flag(t_chunk *c, size_t f)
{
	c->size |= f;
}
static inline void	clear_flag(t_chunk *c, size_t f)
{
	c->size &= ~f;
}


/*	arena.c	   */
t_arena			*arena_create(size_t need);
t_arena			*arena_extend(t_arena *a, size_t need);
void			arena_destroy(t_arena *a);
t_arena			*find_arena_by_ptr(void *ptr);

/*	chunk.c	   */

/*	utils.c    */
size_t			page_size(void);
void			putstr_safe(const char *s);
void			puthex_safe(uintptr_t addr);
void			putnbr_safe(size_t n);

/*	debug.c    */


/*	alloc_core.c	*/

#endif
