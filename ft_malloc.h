/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 21:37:19 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/13 02:13:50 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdint.h>
# include <unistd.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <pthread.h>
# include "src/libft/libft.h"

# define TINY_MAX 	128
# define SMALL_MAX	1024
# define ALIGNMENT	16
# define NBLOCKS	128


typedef union	u_mhead
{
	uint64_t	mh_align[2];
	struct	{
		uint8_t	m_alloc;
		uint8_t	m_index;
		uint16_t	m_magic2;
		uint32_t	m_nbytes;
		uint8_t		m_magic[8];
	} s_minfo;
} t_mhead;

typedef struct s_zone
{
	void			*start;
	void			*end;
	size_t			block_size;
	size_t			total_blocks;
	size_t			used_blocks;
	struct	s_zone	*next;
	t_mhead			*free_list;
} t_zone;

typedef enum e_ztype
{
	Z_TINY,
	Z_SMALL,
	Z_LARGE
}	t_ztype;

//			ft_malloc.c
void	*malloc(size_t size);


//			ft_free.c
void	free(void *ptr);


//			ft_realloc.c
void	*realloc(void *ptr, size_t size);

/*		ft_show_alloc_mem.c    */
void	ft_show_alloc_mem(void);


void	*calloc(size_t count, size_t size);

# endif

