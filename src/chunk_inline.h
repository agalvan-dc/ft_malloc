/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_inline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_INLINE_H
# define CHUNK_INLINE_H

# define HEADER_SIZE 16
# define CHUNK_MASK 0xFFFFFFFFFFFFFFF0

typedef struct s_chunk
{
	size_t			size;
	struct s_chunk	*next;
	struct s_chunk	*prev;
}	t_chunk;

static inline t_chunk	*next_chunk(t_chunk *c)
{
	return ((t_chunk *)((char *)c + HEADER_SIZE + (c->size & CHUNK_MASK)));
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

static inline void	push_front(t_chunk **head, t_chunk *c)
{
	c->prev = NULL;
	c->next = *head;
	if (*head)
		(*head)->prev = c;
	*head = c;
}

#endif
