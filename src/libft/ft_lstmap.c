/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caperale <caperale@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:48:57 by caperale          #+#    #+#             */
/*   Updated: 2026/05/21 14:12:02 by caperale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*aux;
	void	*a;

	if (!lst || !f || !del)
		return (NULL);
	res = NULL;
	while (lst)
	{
		a = f(lst->content);
		aux = ft_lstnew(a);
		if (!aux)
		{
			del(a);
			ft_lstclear(&res, del);
			return (NULL);
		}
		ft_lstadd_back(&res, aux);
		lst = lst->next;
	}
	return (res);
}
