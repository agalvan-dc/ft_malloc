/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_basic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	test_align(void)
{
	char	*p;
	int		i;

	i = 0;
	while (i < 500)
	{
		p = (char *)malloc((size_t)i * 7 + 3);
		CHECK(p != NULL);
		if (p)
			CHECK(((uintptr_t)p & 0xF) == 0);
		free(p);
		i++;
	}
}

static void	test_contents(void)
{
	char	*p;
	size_t	sz;
	int		i;
	int		j;

	j = 0;
	while (j < 40)
	{
		sz = (size_t)(RAND() % 300) + 1;
		p = (char *)malloc(sz);
		CHECK(p != NULL);
		if (!p)
			return ;
		i = 0;
		while (i < (int)sz)
		{
			p[i] = (char)i;
			i++;
		}
		i = 0;
		while (i < (int)sz)
		{
			CHECK(p[i] == (char)i);
			i++;
		}
		free(p);
		j++;
	}
}

static void	test_distinct(void)
{
	void	*p1;
	void	*p2;
	void	*p3;

	p1 = malloc(24);
	p2 = malloc(24);
	p3 = malloc(24);
	CHECK(p1 != NULL && p2 != NULL && p3 != NULL);
	if (p1 && p2 && p3)
		CHECK(p1 != p2 && p2 != p3 && p1 != p3);
	free(p1);
	free(p2);
	free(p3);
}

int	main(void)
{
	g_rng = 20260926;
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_align();
	test_contents();
	test_distinct();
	if (g_fail)
	{
		fprintf(stderr, "t_basic FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_basic PASS\n");
	return (0);
}