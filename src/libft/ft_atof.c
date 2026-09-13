/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalvan- <agalvan-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:46:05 by agalvan-          #+#    #+#             */
/*   Updated: 2026/09/13 01:46:11 by agalvan-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float ft_atof(const char *s)
{
    float sign = 1.0f;
    float num = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;

    while (*s == ' ' || *s == '\t' || *s == '\n' || 
           *s == '\r' || *s == '\f' || *s == '\v')
        s++;
    if (*s == '-' || *s == '+')
    {
        if (*s == '-')
            sign = -1.0f;
        s++;
    }
    while (*s >= '0' && *s <= '9')
    {
        num = num * 10.0f + (*s - '0');
        s++;
    }
    if (*s == '.' || *s == ',')
    {
        s++;
        while (*s >= '0' && *s <= '9')
        {
            fraction = fraction * 10.0f + (*s - '0');
            divisor *= 10.0f;
            s++;
        }
    }
    return (sign * (num + (fraction / divisor)));
}
