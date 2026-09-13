/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:48:35 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 14:10:12 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*ongoing;
	t_list	*tmp;

	if (!lst || !del)
		return ;
	ongoing = *lst;
	while (ongoing)
	{
		del(ongoing->content);
		tmp = ongoing;
		ongoing = ongoing->next;
		free(tmp);
	}
	*lst = NULL;
}
