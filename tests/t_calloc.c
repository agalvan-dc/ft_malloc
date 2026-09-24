/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_calloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	test_zeroing(void)
{
	size_t	*data;
	int		i;

	data = (size_t *)calloc(4096, sizeof(size_t));
	CHECK(data != NULL);
	if (!data)
		return ;
	i = 0;
	while (i < 4096)
	{
		CHECK(data[i] == 0);
		i++;
	}
	free(data);
}

static void	test_overflow(void)
{
	void	*p;

	p = calloc(SIZE_MAX, 2);
	CHECK(p == NULL);
}

static void	test_many(void)
{
	char	*p;
	int		i;

	i = 0;
	while (i < 200)
	{
		p = (char *)calloc(23, 1);
		CHECK(p != NULL);
		if (p)
			CHECK(p[0] == 0 && p[22] == 0);
		free(p);
		i++;
	}
}

static void	test_zero_sizes(void)
{
	void	*a;
	void	*b;

	a = calloc(0, 0);
	b = calloc(0, 8);
	CHECK(a != NULL);
	CHECK(b != NULL);
	free(a);
	free(b);
}

int	main(void)
{
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_zeroing();
	test_overflow();
	test_many();
	test_zero_sizes();
	if (g_fail)
	{
		fprintf(stderr, "t_calloc FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_calloc PASS\n");
	return (0);
}