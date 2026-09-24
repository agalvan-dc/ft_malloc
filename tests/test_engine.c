/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

typedef struct s_entry
{
	const void	*ptr;
	size_t		size;
}	t_entry;

# define MAX_ENT 65536

static t_entry		g_ent[MAX_ENT];
static int			g_n;
static pthread_mutex_t	g_elk = PTHREAD_MUTEX_INITIALIZER;

int		g_ok;
int		g_fail;
uint64_t	g_rng;

void	track_add(const void *ptr, size_t size)
{
	pthread_mutex_lock(&g_elk);
	if (g_n < MAX_ENT)
	{
		g_ent[g_n].ptr = ptr;
		g_ent[g_n].size = size;
		g_n++;
	}
	pthread_mutex_unlock(&g_elk);
}

void	track_remove(const void *ptr)
{
	int	i;

	pthread_mutex_lock(&g_elk);
	i = 0;
	while (i < g_n)
	{
		if (g_ent[i].ptr == ptr)
		{
			g_ent[i] = g_ent[g_n - 1];
			g_n--;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&g_elk);
}

void	track_check(const void *ptr)
{
	const void	*p;
	size_t		size;
	uint64_t	head;
	uint64_t	back;
	int			i;

	pthread_mutex_lock(&g_elk);
	i = 0;
	while (i < g_n)
	{
		if (g_ent[i].ptr == ptr)
		{
			p = ptr;
			size = g_ent[i].size;
			head = *(uint64_t *)((char *)p - GHEAD);
			back = *(uint64_t *)((char *)p + size);
			if (head != CANARY || back != CANARY)
			{
				g_fail++;
				fprintf(stderr, "engine canary break %s:%d\n", __FILE__,
					__LINE__);
			}
			else
				g_ok++;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&g_elk);
}

int	track_live(void)
{
	int	n;

	pthread_mutex_lock(&g_elk);
	n = g_n;
	pthread_mutex_unlock(&g_elk);
	return (n);
}

void	track_reset(void)
{
	pthread_mutex_lock(&g_elk);
	g_n = 0;
	pthread_mutex_unlock(&g_elk);
}