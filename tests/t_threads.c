/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_threads.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	cycle(char **slots, size_t *szs, uint64_t *seed)
{
	int	i;
	int	k;

	i = (int)(RANDV(*seed) % 128);
	k = (int)(RANDV(*seed) % 128);
	if (!slots[i] && (k % 3))
	{
		szs[i] = (size_t)(RANDV(*seed) % 400) + 1;
		GALLOC(slots[i], szs[i]);
		if (slots[i])
		{
			memset(slots[i], k, szs[i]);
			track_add(slots[i], szs[i]);
		}
	}
	else if (slots[i])
	{
		GCHECK(slots[i]);
		track_check(slots[i]);
		track_remove(slots[i]);
		free(slots[i]);
		slots[i] = (void *)0;
	}
}

static void	flush(char **slots)
{
	int	i;

	i = 0;
	while (i < 128)
	{
		if (slots[i])
		{
			GCHECK(slots[i]);
			track_remove(slots[i]);
			free(slots[i]);
			slots[i] = (void *)0;
		}
		i++;
	}
}

static void	*worker(void *arg)
{
	char	*slots[128];
	size_t	szs[128];
	uint64_t	seed;
	int		k;

	(void)arg;
	seed = (uint64_t)(uintptr_t)arg + 1;
	memset(slots, 0, sizeof(slots));
	memset(szs, 0, sizeof(szs));
	k = 0;
	while (k < 8000)
	{
		cycle(slots, szs, &seed);
		k++;
	}
	flush(slots);
	return (NULL);
}

int	main(void)
{
	pthread_t	th[8];
	int			i;

	track_reset();
	g_ok = 0;
	g_fail = 0;
	i = 0;
	while (i < 8)
	{
		pthread_create(&th[i], NULL, worker, (void *)(uintptr_t)(i + 1));
		i++;
	}
	i = 0;
	while (i < 8)
	{
		pthread_join(th[i], NULL);
		i++;
	}
	CHECK(track_live() == 0);
	if (g_fail)
	{
		fprintf(stderr, "t_threads FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_threads PASS\n");
	return (0);
}