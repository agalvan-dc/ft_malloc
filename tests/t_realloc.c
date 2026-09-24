/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_realloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	test_grow(void)
{
	char	*p;
	int		i;

	p = (char *)malloc(33);
	CHECK(p != NULL);
	i = 0;
	while (i < 33)
	{
		p[i] = (char)(i * 2);
		i++;
	}
	p = (char *)realloc(p, 512);
	CHECK(p != NULL);
	i = 0;
	while (i < 33)
	{
		CHECK(p[i] == (char)(i * 2));
		i++;
	}
	free(p);
}

static void	test_shrink(void)
{
	char	*p;
	int		i;

	p = (char *)malloc(1000);
	CHECK(p != NULL);
	i = 0;
	while (i < 1000)
	{
		p[i] = 'x';
		i++;
	}
	p = (char *)realloc(p, 17);
	CHECK(p != NULL);
	if (p)
	{
		CHECK(p[0] == 'x');
		CHECK(p[16] == 'x');
	}
	free(p);
}

static void	test_combine(void)
{
	char	*a;
	char	*b;
	char	*q;
	int		i;

	a = (char *)malloc(48);
	b = (char *)malloc(32);
	CHECK(a != NULL && b != NULL);
	i = 0;
	while (i < 48)
	{
		a[i] = (char)i;
		i++;
	}
	free(b);
	q = (char *)realloc(a, 96);
	CHECK(q != NULL);
	if (q)
	{
		i = 0;
		while (i < 48)
		{
			CHECK(q[i] == (char)i);
			i++;
		}
	}
	free(q);
}

static void	test_edge_cases(void)
{
	void	*p;
	char	*c;

	p = malloc(64);
	CHECK(p != NULL);
	p = realloc(p, 0);
	CHECK(p == NULL);
	c = (char *)realloc(NULL, 100);
	CHECK(c != NULL);
	if (c)
	{
		CHECK(((uintptr_t)c & 0xF) == 0);
		free(c);
	}
}

int	main(void)
{
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_grow();
	test_shrink();
	test_combine();
	test_edge_cases();
	if (g_fail)
	{
		fprintf(stderr, "t_realloc FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_realloc PASS\n");
	return (0);
}