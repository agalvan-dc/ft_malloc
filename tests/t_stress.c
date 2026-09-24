/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_stress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	stress_one(void)
{
	void	*slots[256];
	size_t	szs[256];
	int		i;
	int		k;

	memset(slots, 0, sizeof(slots));
	memset(szs, 0, sizeof(szs));
	k = 0;
	while (k < 20000)
	{
		i = (int)(RAND() % 256);
		if (!slots[i] && (RAND() % 4))
		{
			szs[i] = (size_t)(RAND() % 500) + 1;
			GALLOC(slots[i], szs[i]);
			if (slots[i])
			{
				memset(slots[i], (int)(RAND() & 0xFF), szs[i]);
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
		k++;
	}
	i = 0;
	while (i < 256)
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

static void	stress_two(void)
{
	void	*a;
	void	*b;
	int		i;

	i = 0;
	while (i < 50)
	{
		a = malloc((size_t)(RAND() % 200) + 1);
		b = malloc((size_t)(RAND() % 300) + 1);
		if (a && b)
			CHECK((char *)a != (char *)b);
		free(a);
		free(b);
		i++;
	}
}

static void	stress_three(void)
{
	char	*p[4];
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		p[i] = NULL;
		i++;
	}
	j = 0;
	while (j < 500)
	{
		i = (int)(RAND() % 4);
		if (!p[i])
			p[i] = (char *)malloc((size_t)(RAND() % 1000) + 1);
		if (p[i])
			p[i][0] = (char)j;
		j++;
	}
	i = 0;
	while (i < 4)
	{
		free(p[i]);
		i++;
	}
}

int	main(void)
{
	g_rng = 777;
	track_reset();
	g_ok = 0;
	g_fail = 0;
	stress_one();
	stress_two();
	stress_three();
	if (g_fail)
	{
		fprintf(stderr, "t_stress FAIL\n");
		return (1);
	}
	fprintf(stderr, "t_stress PASS\n");
	return (0);
}