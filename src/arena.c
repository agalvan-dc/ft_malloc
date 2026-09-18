/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 arena.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: agalvan- <agalvan-@student.42.es>			+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/09/15 13:23:33 by agalvan-		   #+#	  #+#			  */
/*	 Updated: 2026/09/16 00:00:06 by agalvan-		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "internal.h"

__thread t_arena	*g_cache = NULL;
t_arena			*g_all_arenas = NULL;
pthread_mutex_t		g_list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t		g_large_lock = PTHREAD_MUTEX_INITIALIZER;
size_t			g_dyn_threshold = DYNAMIC_THRESHOLD;
static pthread_t	g_main_thread;
static int		g_main_init = 0;


static	void	*arena_map(size_t size, int use_brk)
{
	void	*base;

	base = NULL;
	if (use_brk)
	{
	base = sbrk(size);
	if (base == (void *)-1)
		base = NULL;
	}
	if (!base)
	{
	base = mmap(NULL, size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (base == MAP_FAILED)
		base = NULL;
	}
	return (base);
}

static void	arena_init(t_arena *a, size_t size, int use_brk)
{
   t_chunk	*top;
	size_t	payload;

	pthread_mutex_init(&a->mutex, NULL);
	a->free_list = NULL;
	a->total = size;
	a->used = 0;
	a->capacity = ARENA_MAX_PAGES * page_size();
	a->is_brk = use_brk;
	payload = size - sizeof(t_arena) - HEADER_SIZE;
	top->size = payload | CHUNK_FREE | CHUNK_ZEROED;
	top->next = NULL;
	a->top = top *(size_t)((char *)top + HEADER_SIZE + payload - sizeof(size_t)) = payload;
	a->next = g_all_arenas;
	g_all_arenas = a;
	pthread_mutex_unlock(&g_list_lock);
	g_cache = a;
}

t_arena	*arena_create(size_t need)
{  
	void	*base;
	size_t	size;
	int		use_brk;

	need = align_up(need, page_size());
	size = ARENA_INIT_PAGES * page_size();
	if (need > size)
		size = need;
	if (size > ARENA_MAX_PAGES * page_size())
		return (NULL);
	if (!g_main_init)
	{
		g_main_thread = pthread_self();
		g_main_init = 1;
	}
	use_brk = (pthread_self() == g_main_thread);
	base = arena_map(size, use_brk);
	if (!base)
		return (NULL);
	arena_init((t_arena *)base, size, use_brk);
	return ((t_arena *)base);
}


