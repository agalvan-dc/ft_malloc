/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_robust.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	test_double_free(void)
{
	void	*p;

	p = malloc(64);
	CHECK(p != NULL);
	free(p);
	free(p);
	CHECK(1);
}

static void	test_bad_frees(void)
{
	int	local;
	void	*p;

	local = 42;
	free(NULL);
	free(&local);
	free((void *)0x1);
	p = malloc(32);
	CHECK(p != NULL);
	free(p);
	free(p);
	free(NULL);
	CHECK(track_live() == 0);
}

static void	test_bad_realloc(void)
{
	int		local;
	void	*p;

	local = 7;
	p = realloc((void *)&local, 100);
	CHECK(p == NULL);
	(void)p;
}

int	main(void)
{
	track_reset();
	g_ok = 0;
	g_fail = 0;
	test_double_free();
	test_bad_frees();
	test_bad_realloc();
	if (g_fail)
	{
		fprintf(stderr, "t_robust FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_robust PASS\n");
	return (0);
}