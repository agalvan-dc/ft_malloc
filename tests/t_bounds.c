/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_bounds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	test_neighbors(void)
{
	char	*a;
	char	*b;
	char	*c;

	GALLOC(a, 64);
	GALLOC(b, 64);
	GALLOC(c, 64);
	if (a && b && c)
	{
		memset(a, 'A', 64);
		memset(b, 'B', 64);
		memset(c, 'C', 64);
		free(b);
		GCHECK(a);
		GCHECK(c);
		CHECK(a[0] == 'A');
		CHECK(c[0] == 'C' && c[63] == 'C');
	}
	free(a);
	free(c);
}

static void	test_merge_reuse(void)
{
	void	*p;
	void	*q;
	int		i;

	p = malloc(100);
	q = malloc(100);
	CHECK(p != NULL && q != NULL);
	free(p);
	free(q);
	i = 0;
	while (i < 20)
	{
		p = malloc(220);
		CHECK(p != NULL);
		free(p);
		i++;
	}
}

static void	test_shrink_payload(void)
{
	char	*a;
	char	*b;
	int		i;

	a = (char *)malloc(256);
	CHECK(a != NULL);
	i = 0;
	while (i < 256)
	{
		a[i] = 'z';
		i++;
	}
	a = (char *)realloc(a, 128);
	CHECK(a != NULL);
	b = (char *)malloc(64);
	CHECK(b != NULL);
	if (a && b)
	{
		CHECK(a[0] == 'z' && a[127] == 'z');
		memset(b, 'y', 64);
		CHECK(a[0] == 'z' && a[127] == 'z');
	}
	free(a);
	free(b);
}

int	main(void)
{
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_neighbors();
	test_merge_reuse();
	test_shrink_payload();
	if (g_fail)
	{
		fprintf(stderr, "t_bounds FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_bounds PASS\n");
	return (0);
}