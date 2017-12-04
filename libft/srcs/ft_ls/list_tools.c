/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/01 00:18:26 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/13 15:06:42 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		sort_ascii(t_dircont *c1, void *c2)
{
	return (ft_strcmp(c1->name, ((t_dircont*)c2)->name));
}

void	sort_list(t_list **dir, int o, int (*sort)(t_dircont *,
			void *, int))
{
	t_list		*tmp;
	t_list		*swap2;
	t_dircont	*dc;

	tmp = *dir;
	swap2 = NULL;
	if (!tmp || !tmp->next)
		return ;
	swap2 = tmp->next;
	while (swap2)
	{
		dc = tmp->content;
		if (sort(dc, swap2->content, o) > 0)
		{
			tmp->content = swap2->content;
			swap2->content = dc;
			tmp = *dir;
			swap2 = tmp->next;
		}
		else
		{
			tmp = tmp->next;
			swap2 = tmp->next;
		}
	}
}

void	sort_select(t_list **dir, void *content, size_t cont_size)
{
	t_list	*new;

	new = NULL;
	if (!(new = ft_lstnew(content, cont_size)))
		return ;
	if (!(*dir))
		*dir = new;
	else
		ft_lst_sortinsert(dir, new, sort_ascii);
}
