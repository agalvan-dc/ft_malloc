/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 21:37:19 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/15 15:30:11 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdint.h>
# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>
# include "src/libft/libft.h"


# define ALIGNMENT		16
# define HEADER_SIZE		(sizeof(t_chunk))
# define MIN_SPLIT		32

# define LARGE_BASE_THRESHOLD	2048
# define DYNAMIC_THRESHOLD	(64 * 1024)
# define THRESHOLD_MAX		(4 * 1024 * 1024)
# define LARGE_CACHE_CAP	(4 * 1024 * 1024)
# define NUM_LARGE_BUCKETS	15

# define ARENA_INIT_PAGES	16
# define ARENA_MAX_PAGES	256

# define CHUNK_FREE		0x1
# define CHUNK_PREV_FREE	0x2
# define CHUNK_ZEROED		0x4
# define CHUNK_MMAPPED		0x8
# define CHUNK_MASK		(~0XF)


typedef struct s_chunk
{
    size_t		size;
    struct s_chunk	*next;
} t_chunk;


typedef struct	s_arena
{
    pthread_mutex_t	mutex;
    t_chunk		*free_list;
    t_chunk		*top;
    size_t		total;
    size_t		used;
    size_t		capacity;
    int			is_brk;
    struct s_arena	*next;
}  __attribute__((aligned(64))) t_arena;


void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);
void	ft_show_alloc_mem(void);

# endif

