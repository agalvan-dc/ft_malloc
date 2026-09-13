/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 13:31:55 by caperale          #+#    #+#             */
/*   Updated: 2026/05/27 22:11:21 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_pf(char *str, int *counter)
{
	if (!str)
		str = ("(null)");
	while (*str)
		ft_putchar_pf(*str++, counter);
}

void	ft_putnbr_pf(int n, int *counter)
{
	if (n < 0)
	{
		ft_putchar_pf('-', counter);
		if (n == -2147483648)
		{
			ft_putchar_pf('2', counter);
			n = -147483648;
		}
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_pf(n / 10, counter);
	ft_putchar_pf((n % 10) + '0', counter);
}

void	ft_putu_pf(unsigned int n, int *counter)
{
	if (n >= 10)
		ft_putu_pf(n / 10, counter);
	ft_putchar_pf((n % 10) + '0', counter);
}

void	ft_putptr(void *ptr, int *counter)
{
	unsigned long	p;

	p = (unsigned long)ptr;
	if (!ptr)
		return (ft_putstr_pf(("(nil)"), counter));
	ft_putstr_pf("0x", counter);
	ft_puthex_pf(p, counter, 'x');
}

void	ft_puthex_pf(unsigned long n, int *counter, const char format)
{
	char	*bs;

	if (format == 'X')
		bs = "0123456789ABCDEF";
	else
		bs = "0123456789abcdef";
	if (n == 0)
	{
		ft_putchar_pf('0', counter);
		return ;
	}
	if (n >= 16)
		ft_puthex_pf(n / 16, counter, format);
	ft_putchar_pf(bs[n % 16], counter);
}
