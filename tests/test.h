/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 00:00:00 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/26 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>

# define CANARY 0xDEADBEEFCAFEF00DULL
# define GHEAD 16
# define GTAIL 16

extern int	g_ok;
extern int	g_fail;
extern uint64_t	g_rng;

# define RAND() (g_rng = g_rng * 6364136223846793005ULL + 1ULL, (g_rng >> 33))
# define RANDV(v) (v = v * 6364136223846793005ULL + 1ULL, (v >> 33))

# define CHECK(cond) do { if (cond) g_ok++; else { g_fail++; \
	fprintf(stderr, "CHECK failed %s:%d\n", __FILE__, __LINE__); } } while (0)

# define GALLOC(p, sz) do { char *_g; \
	_g = (char *)malloc((sz) + GHEAD + GTAIL); \
	if (!_g) { g_fail++; p = (void *)0; break; } \
	*(uint64_t *)_g = CANARY; \
	*(uint64_t *)(_g + 8) = (uint64_t)(sz); \
	*(uint64_t *)(_g + GHEAD + (sz)) = CANARY; \
	p = (void *)(_g + GHEAD); } while (0)

# define GSIZE(p) (*(uint64_t *)((char *)(p) - 8))

# define GCHECK(p) do { uint64_t _h; uint64_t _b; \
	_h = *(uint64_t *)((char *)(p) - GHEAD); \
	_b = *(uint64_t *)((char *)(p) + GSIZE(p)); \
	if (_h != CANARY || _b != CANARY) { g_fail++; \
		fprintf(stderr, "CANARY broken %s:%d\n", __FILE__, __LINE__); } \
	else g_ok++; } while (0)

void	track_add(const void *ptr, size_t size);
void	track_remove(const void *ptr);
void	track_check(const void *ptr);
int		track_live(void);
void	track_reset(void);

#endif