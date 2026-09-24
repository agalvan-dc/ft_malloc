/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_large.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include "../ft_malloc.h"

static void	test_small_arena(void)
{
	char	*p;

	p = (char *)malloc(60000);
	CHECK(p != NULL);
	if (p)
	{
		memset(p, 'L', 60000);
		CHECK(p[0] == 'L');
		CHECK(p[59999] == 'L');
	}
	free(p);
}

static void	test_large_mmap(void)
{
	char	*p;

	p = (char *)malloc(100000);
	CHECK(p != NULL);
	if (p)
	{
		memset(p, 'B', 100000);
		CHECK(p[0] == 'B');
		CHECK(p[99999] == 'B');
	}
	free(p);
	p = (char *)malloc(300000);
	CHECK(p != NULL);
	if (p)
	{
		memset(p, 'C', 300000);
		CHECK(p[299999] == 'C');
	}
	free(p);
}

static void	test_large_realloc(void)
{
	char	*p;
	char	*q;

	p = (char *)malloc(90000);
	CHECK(p != NULL);
	if (p)
		memset(p, 'R', 90000);
	q = (char *)realloc(p, 150000);
	CHECK(q != NULL);
	if (p && q)
		CHECK(q[0] == 'R' && q[89999] == 'R');
	free(q);
}

static void	test_huge_fail(void)
{
	CHECK(malloc(SIZE_MAX) == NULL);
	CHECK(malloc((size_t)-1 - 100) == NULL);
}

static void	test_mid_range(void)
{
	void	*p;
	int		i;

	i = 1;
	while (i < 40)
	{
		p = malloc((size_t)i * 1024 + 1);
		CHECK(p != NULL);
		free(p);
		i++;
	}
	p = malloc(0);
	CHECK(p != NULL);
	free(p);
}

int	main(void)
{
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_small_arena();
	test_large_mmap();
	test_large_realloc();
	test_huge_fail();
	test_mid_range();
	show_alloc_mem();
	if (g_fail)
	{
		fprintf(stderr, "t_large FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_large PASS\n");
	return (0);
}