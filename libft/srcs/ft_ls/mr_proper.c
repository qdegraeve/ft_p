/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mr_proper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/08 18:05:03 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/14 18:41:29 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_ls.h"

void	ft_lst_sortinsert(t_list **beg_lst, t_list *new,
		int (*sort)(t_dircont *, void *))
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	tmp = *beg_lst;
	while (tmp && sort(((t_dircont*)new->content), tmp->content) > 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		tmp2->next = new;
	else if (tmp == *beg_lst)
	{
		new->next = tmp;
		*beg_lst = new;
	}
	else
	{
		tmp = tmp2;
		new->next = tmp->next;
		tmp->next = new;
	}
}

void	ft_lst_insert(t_list **lst, t_list *in)
{
	t_list	*tmp;

	tmp = NULL;
	if (!*lst)
		*lst = in;
	else
	{
		tmp = in;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (*lst)->next;
		(*lst)->next = in;
	}
	tmp = *lst;
}

void	del_dircont(void *to_del)
{
	t_dircont	*dc;

	dc = to_del;
	if (dc->name)
		free(dc->name);
	if (dc->type)
		free(dc->type);
	if (dc->path)
		free(dc->path);
	if (to_del)
		free(to_del);
}

void	del_string(void *to_del)
{
	char	*tmp;

	tmp = to_del;
	ft_strdel(&tmp);
}

void	lst_delone(t_list **lst, void (*del)(void *))
{
	t_list		*tmp;

	tmp = NULL;
	tmp = (*lst)->next;
	del((*lst)->content);
	free(*lst);
	*lst = tmp;
}
