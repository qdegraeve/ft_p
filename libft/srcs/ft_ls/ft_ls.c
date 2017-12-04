/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/24 16:31:35 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/14 18:41:44 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	init_display(t_display *d)
{
	d->len_max = 0;
	d->link_max = 0;
	d->owner_max = 0;
	d->group_max = 0;
	d->size_max = 0;
	d->total = 0;
	d->sys = 0;
}


char		**ft_ls(char **args)
{
	t_options	o;
	t_display	d;
	t_list		*lst;
	char		**ret;

	lst = NULL;
	ret = NULL;
	init_options(&o);
	init_display(&d);
	check_options(args, &o);
	ft_lstadd_back(&lst, ".", 2);
	d.o = &o;
	while (lst)
	{
		ret = open_dir(lst->content, d);
		lst_delone(&lst, del_string);
	}
	return (ret);
}