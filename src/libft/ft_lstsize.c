/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:49:06 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 14:13:33 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*ongoing;
	int		i;

	ongoing = lst;
	i = 0;
	while (ongoing)
	{
		i++;
		ongoing = ongoing->next;
	}
	return (i);
}
