/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42.es>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:51:23 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/23 00:00:00 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

size_t	page_size(void)
{
	static size_t	cached;

	if (!cached)
		cached = sysconf(_SC_PAGESIZE);
	return (cached);
}

void	putstr_safe(const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	write(1, s, len);
}

void	puthex_safe(uintptr_t addr)
{
	char		buf[17];
	const char	*hex;
	int			i;

	hex = "0123456789abcdef";
	putstr_safe("0x");
	i = 17;
	if (addr == 0)
		buf[--i] = '0';
	while (addr > 0)
	{
		buf[--i] = hex[addr % 16];
		addr /= 16;
	}
	write(1, buf + i, 17 - i);
}

void	putnbr_safe(size_t n)
{
	char	buf[21];
	int		i;

	i = 21;
	if (n == 0)
		buf[--i] = '0';
	while (n > 0)
	{
		buf[--i] = '0' + (n % 10);
		n /= 10;
	}
	write(1, buf + i, 21 - i);
}
