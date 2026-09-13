/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 13:32:03 by caperale          #+#    #+#             */
/*   Updated: 2026/05/27 22:11:32 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_pf(char c, int *counter)
{
	write(1, &c, 1);
	(*counter)++;
}

void	ft_print_pf(char *fmt, int *counter, va_list arglst)
{
	if (*fmt == 'c')
		ft_putchar_pf(va_arg(arglst, int), counter);
	else if (*fmt == '%')
		ft_putchar_pf('%', counter);
	else if (*fmt == 's')
		ft_putstr_pf(va_arg(arglst, char *), counter);
	else if (*fmt == 'i' || *fmt == 'd')
		ft_putnbr_pf(va_arg(arglst, int), counter);
	else if (*fmt == 'u')
		ft_putu_pf(va_arg(arglst, unsigned int), counter);
	else if (*fmt == 'x' || *fmt == 'X')
		ft_puthex_pf(va_arg(arglst, unsigned int), counter, *fmt);
	else if (*fmt == 'p')
		ft_putptr(va_arg(arglst, void *), counter);
}

int	ft_printf(const char *format, ...)
{
	va_list	arglst;
	int		counter;
	char	*fmt;

	counter = 0;
	if (!format)
		return (-1);
	fmt = (char *)format;
	va_start(arglst, format);
	while (*fmt)
	{
		if (*fmt != '%')
			ft_putchar_pf(*fmt, &counter);
		else
		{
			fmt++;
			if (*fmt)
				ft_print_pf(fmt, &counter, arglst);
		}
		fmt++;
	}
	va_end(arglst);
	return (counter);
}

/*#include <stdio.h>
int	main(void)
{
	char			c;
	char			*s;
	int				n;
	unsigned int	u;
	int				hex;

	int				ft_ret;
	int				og_ret;

	c = 'A';
	s = "Hola Mundo";
	n = -42;
	u = 42;
	hex = 255;

	ft_ret = ft_printf(
			"FT_PRINTF -> %c %s %d %i %u %x %X %p %%\n",
			c, s, n, n, u, hex, hex, &n);

	og_ret = printf(
			"PRINTF    -> %c %s %d %i %u %x %X %p %%\n",
			c, s, n, n, u, hex, hex, &n);

	printf("\nCaracteres impresos:\n");
	printf("ft_printf: %d\n", ft_ret);
	printf("printf   : %d\n", og_ret);
	
	printf("%u\n", printf(NULL));
	ft_printf("%u\n", ft_printf(NULL));
	return (0);
}*/
