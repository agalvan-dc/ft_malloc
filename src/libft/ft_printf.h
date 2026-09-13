/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperales <cperales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:30:22 by cperales          #+#    #+#             */
/*   Updated: 2025/05/11 21:09:07 by cperales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int		ft_printf(const char *format, ...);
void	ft_putchar_pf(char c, int *counter);
void	ft_putstr_pf(char *str, int *counter);
void	ft_putnbr_pf(int n, int *counter);
void	ft_putu_pf(unsigned int n, int *counter);
void	ft_puthex_pf(unsigned long n, int *counter, const char base);
void	ft_putptr(void *ptr, int *counter);

#endif
