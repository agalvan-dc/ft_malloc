/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:35:08 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "../ft_malloc.h"
# include "libft/libft.h"
# include "chunk_inline.h"

# define ALIGNMENT 16
# define MIN_SPLIT 32

# define BIN_COUNT 64
# define BIN_BIG_OFF 64
# define BIN_BIG_COUNT 12
# define BIN_TOTAL 76
# define BIN_MAX_PAYLOAD 1024

# define DYNAMIC_THRESHOLD 65536
# define THRESHOLD_MAX 262144
# define LARGE_CACHE_CAP 4194304
# define NUM_LARGE_BUCKETS 15

# define ARENA_MIN_PAGES 16
# define ARENA_MAX_PAGES 2048
# define ZONE_TINY_MAX 512
# define TINY_MAX ZONE_TINY_MAX

# define CHUNK_FREE 0x1
# define CHUNK_PREV_FREE 0x2
# define CHUNK_ZEROED 0x4
# define CHUNK_MMAPPED 0x8

# define TC_CLASSES 64
# define TC_MAX 1024
# define TC_LIMIT 64
# define TC_HOT_IDX 2
# define TC_HOT_LIMIT 65536
# define TC_OVERFLOW 262144

typedef struct s_tcache
{
	t_chunk			*head[TC_CLASSES];
	unsigned int	len[TC_CLASSES];
}				t_tcache;

typedef struct s_arena
{
	pthread_mutex_t	mutex;
	t_chunk			*bins[BIN_TOTAL];
	unsigned long	dirty[2];
	t_chunk			*big;
	t_chunk			*top;
	size_t			total;
	size_t			used;
	size_t			capacity;
	int				label;
	int				is_brk;
	struct s_arena	*next;
}				t_arena __attribute__((aligned(64)));

typedef struct s_allocator
{
	pthread_mutex_t	list_lock;
	pthread_mutex_t	large_lock;
	t_arena			*all_arenas;
	t_chunk			*large_cache[NUM_LARGE_BUCKETS];
	t_chunk			*large_all;
	size_t			dyn_threshold;
	size_t			large_cached;
	pthread_t		main_thread;
	int				main_init;
}				t_allocator;

extern t_allocator			g_alloc;
extern __thread t_arena		*g_last_arena;

static inline size_t	align_up(size_t n, size_t align)
{
	return ((n + (align - 1)) & ~(align - 1));
}

static inline unsigned int	tc_cap(int idx)
{
	if (idx <= TC_HOT_IDX)
		return (TC_HOT_LIMIT);
	return (TC_LIMIT);
}

static inline unsigned int	tc_tray(int idx)
{
	if (idx <= TC_HOT_IDX)
		return (TC_OVERFLOW);
	return (TC_LIMIT);
}

static inline t_chunk	*chunk_from_ptr(void *ptr)
{
	return ((t_chunk *)((char *)ptr - HEADER_SIZE));
}

static inline void	*payload_from_chunk(t_chunk *c)
{
	return ((void *)((char *)c + HEADER_SIZE));
}

static inline size_t	chunk_size(t_chunk *c)
{
	return (c->size & CHUNK_MASK);
}

static inline int	size2bin(size_t p)
{
	size_t	b;
	int		bin;

	if (p <= BIN_MAX_PAYLOAD)
		return ((int)((p - 16) >> 4));
	b = BIN_MAX_PAYLOAD;
	bin = BIN_BIG_OFF;
	while (p > b && bin < BIN_BIG_OFF + BIN_BIG_COUNT - 1)
	{
		b *= 2;
		bin++;
	}
	return (bin);
}

/*	alloc_core.c	   */
void		bin_push(t_arena *a, t_chunk *c);
t_chunk		*find_free(t_arena *a, size_t need);
t_chunk		*carve_top(t_arena *a, size_t need);
t_chunk		*carve_prefetch(t_arena *a, size_t need);

/*	arena.c		   */
t_arena		*arena_create(size_t need);
size_t		arena_hdr(void);

/*	arena_ops.c	   */
t_arena		*arena_extend(t_arena *a, size_t need);
void		arena_destroy(t_arena *a);
t_arena		*find_arena_by_ptr(void *ptr);
t_arena		*arena_from_ptr(void *ptr);

/*	chunk.c		   */
t_chunk		*chunk_split(t_chunk *c, size_t take);

/*	coalesce.c	   */
void		remove_from_bin(t_arena *a, t_chunk *target);
t_chunk		*coalesce_next(t_arena *a, t_chunk *c);
t_chunk		*coalesce_prev(t_arena *a, t_chunk *c);

/*	free_utils.c   */
void		free_push(t_arena *a, t_chunk *c);

/*	tcache.c	   */
int			tc_index(size_t payload);
int			tc_push(t_arena *a, t_chunk *c);
int			tc_unpark(t_chunk *target);
t_chunk		*tc_pop(size_t need);
t_tcache	*tc_peek(void);
t_tcache	*tc_struct_lazy(void);

/*	large.c		   */
t_chunk		*large_alloc(size_t size);
void		large_free_chunk(t_chunk *c);
void		*large_realloc(t_chunk *c, size_t need);
void		large_print_live(size_t *total);

/*	debug.c		   */
void		show_alloc_mem_ex(void);

/*	utils.c		   */
size_t		page_size(void);
void		putstr_safe(const char *s);
void		puthex_safe(uintptr_t addr);
void		putnbr_safe(size_t n);
int			ptr_plausible(const void *ptr);

#endif
