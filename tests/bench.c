/* ************************************************************************** */
/* ************************************************************************** */
/*   bench.c                                        ft_malloc               */
/*                                                                          */
/*   Micro-benchmark (LD_PRELOAD-friendly, single-threaded):                */
/*                                                                          */
/*     tiny     200k  malloc(32)+write+free                                 */
/*     mixed     50k  malloc(1..512)+write+free                             */
/*     malloc   120k  malloc(32) held in a static array (malloc-only rate)  */
/*     free     120k  free() of that same set (free-only rate)              */
/*     hold      1k   malloc(4096) held + freed                             */
/*     big       3k   malloc(80000) LARGE + write + free                    */
/*     realloc  20k   malloc(1000) -> realloc(4..16 KB) -> free             */
/*                                                                          */
/*   The malloc and free phases are isolated on purpose: `malloc` times     */
/*   only the allocation burst (all pointers kept so the compiler cannot    */
/*   elide them) and `free` times only the deallocation burst of that      */
/*   pre-filled set.                                                        */
/*                                                                          */
/*   Memory comes from /proc/self/status (VmRSS current, VmHWM high-water) */
/*   instead of getrusage(): this kernel returns a bogus ru_maxrss (~1.1 GB)  */
/*   as soon as an anonymous mmap happens, while ru_maxrss before any map   */
/*   is accurate. /proc VmHWM/VmRSS are kernel-truthful on both runs.       */
/*                                                                          */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define N_PTRS	120000

static void				*g_ptrs[N_PTRS];
static volatile unsigned long	g_sink;

typedef struct s_meminfo
{
	unsigned long	rss;
	unsigned long	peak;
}				t_meminfo;

static unsigned long	now_us(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts))
		return (0);
	return (ts.tv_sec * 1000000UL + ts.tv_nsec / 1000UL);
}

static unsigned long	proc_value(const char *key)
{
	char		buf[4096];
	char		*p;
	ssize_t		got;
	int			fd;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd < 0)
		return (0);
	got = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (got <= 0)
		return (0);
	buf[got] = '\0';
	p = strstr(buf, key);
	if (!p)
		return (0);
	p += strlen(key);
	while (*p == ' ' || *p == '\t')
		p++;
	return ((unsigned long)strtoul(p, NULL, 10));
}

static t_meminfo	get_meminfo(void)
{
	t_meminfo	m;

	m.rss = proc_value("VmRSS:");
	m.peak = proc_value("VmHWM:");
	return (m);
}

static void	run_phase(const char *name, void (*fn)(void))
{
	t_meminfo	m;
	unsigned long	t0;
	unsigned long	t1;

	t0 = now_us();
	fn();
	t1 = now_us();
	m = get_meminfo();
	printf("%-10s %9lu us  rss=%-6lukB peak=%-6lukB\n",
		   name, t1 - t0, m.rss, m.peak);
}

static void	phase_tiny(void)
{
	void	*p;
	int		i;

	i = 0;
	while (i < 200000)
	{
		p = malloc(32);
		if (!p)
			return ;
		memset(p, 't', 32);
		g_sink ^= (unsigned long)p;
		free(p);
		i++;
	}
}

static void	phase_mixed(void)
{
	void	*p;
	int		i;
	int		n;

	i = 0;
	while (i < 50000)
	{
		n = (int)((i * 2654435761u) % 512) + 1;
		p = malloc((size_t)n);
		if (!p)
			return ;
		memset(p, 'm', (size_t)n);
		g_sink ^= (unsigned long)p;
		free(p);
		i++;
	}
}

static void	phase_malloc(void)
{
	void	*p;
	int		i;

	i = 0;
	while (i < N_PTRS)
	{
		p = malloc(32);
		if (!p)
			return ;
		g_sink ^= (unsigned long)p;
		g_ptrs[i] = p;
		i++;
	}
}

static void	phase_free(void)
{
	int	i;

	i = 0;
	while (i < N_PTRS)
	{
		g_sink ^= (unsigned long)g_ptrs[i];
		free(g_ptrs[i]);
		i++;
	}
}

static void	phase_hold(void)
{
	void	*p;
	int		i;

	i = 0;
	while (i < 1000)
	{
		p = malloc(4096);
		if (!p)
			return ;
		memset(p, 'h', 4096);
		g_sink ^= (unsigned long)p;
		free(p);
		i++;
	}
}

#define WASTE_N	50000
#define WASTE_SZ	32

static void	phase_waste_free(void)
{
	int	i;

	i = 0;
	while (i < WASTE_N)
	{
		g_sink ^= (unsigned long)g_ptrs[i];
		free(g_ptrs[i]);
		g_ptrs[i] = NULL;
		i++;
	}
}

static void	phase_waste(void)
{
	int		i;
	unsigned long	useful;
	t_meminfo	m;

	i = 0;
	while (i < WASTE_N)
	{
		g_ptrs[i] = malloc(WASTE_SZ);
		if (!g_ptrs[i])
			return ;
		memset(g_ptrs[i], 'w', WASTE_SZ);
		g_sink ^= (unsigned long)g_ptrs[i];
		i++;
	}
	m = get_meminfo();
	useful = (unsigned long)WASTE_N * WASTE_SZ;
	if (m.rss > 0)
		printf("        waste=%.1f%%  (rss %lukB, útil %lukB)\n",
			   100.0 - (100.0 * (double)useful / 1024.0) / (double)m.rss,
			   m.rss, useful / 1024);
	phase_waste_free();
}

static void	phase_big(void)
{
	void	*p;
	int		i;

	i = 0;
	while (i < 3000)
	{
		p = malloc(80000);
		if (!p)
			return ;
		memset(p, 'b', 80000);
		g_sink ^= (unsigned long)p;
		free(p);
		i++;
	}
}

static void	phase_realloc(void)
{
	void	*p;
	int		i;
	int		sz;

	i = 0;
	while (i < 20000)
	{
		p = malloc(1000);
		if (!p)
			return ;
		sz = (i % 4 + 1) * 4000;
		p = realloc(p, (size_t)sz);
		if (!p)
			return ;
		memset(p, 'r', (size_t)sz);
		g_sink ^= (unsigned long)p;
		free(p);
		i++;
	}
}

int	main(void)
{
	printf("=== ft_malloc bench, pid=%d ===\n", (int)getpid());
	run_phase("tiny", phase_tiny);
	run_phase("mixed", phase_mixed);
	run_phase("malloc", phase_malloc);
	run_phase("free", phase_free);
	run_phase("hold", phase_hold);
	run_phase("big", phase_big);
	run_phase("realloc", phase_realloc);
	run_phase("waste", phase_waste);
	return (0);
}
