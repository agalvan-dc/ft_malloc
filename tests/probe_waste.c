/* ************************************************************************** */
/*                                                                            */
/*   probe_waste.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define P_N	50000
#define P_SZ	32

static volatile unsigned long	g_sink;

static unsigned long	rss(void)
{
	char		buf[4096];
	ssize_t		n;
	size_t		len;
	char		*p;
	int			fd;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd < 0)
		return (0);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (n <= 0)
		return (0);
	buf[n] = '\0';
	p = strstr(buf, "VmRSS:");
	if (!p)
		return (0);
	p += 6;
	len = strlen(p);
	return ((unsigned long)strtoul(p, NULL, 10));
}

int	main(void)
{
	void	*p;
	unsigned long useful;
	int		i;
	unsigned long r;

	i = 0;
	while (i < P_N)
	{
		p = malloc(P_SZ);
		if (!p)
			return (1);
		memset(p, 'w', P_SZ);
		g_sink ^= (unsigned long)p;
		i++;
	}
	r = rss();
	useful = (unsigned long)P_N * P_SZ;
	printf("waste=%.1f%%  (rss %lukB, util %lukB)\n",
		    100.0 - (100.0 * (double)useful) / 1024.0 / (double)r / 1.0,
		   r, useful / 1024);
	i = 0;
	while (i < P_N)
	{
		free(g_ptrs_placeholder[(int)i]);
		i++;
	}
	return (0);
}
</content>