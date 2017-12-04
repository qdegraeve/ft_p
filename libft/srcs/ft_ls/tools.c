/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/08 01:57:22 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/13 15:05:50 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_num_len(size_t num)
{
	int		i;

	i = 0;
	if (num == 0)
		return (0);
	while ((num = num / 10))
		i++;
	return (i + 1);
}

int		ft_isnavdir(char *name)
{
	if (!name)
		return (0);
	if (name[0] == '.' && (!name[1] || name[1] == '.'))
		return (1);
	else
		return (0);
}

int		ft_ishidden(char *name)
{
	if (!name)
		return (0);
	if (name[0] == '.' && !ft_isnavdir(name))
		return (1);
	else
		return (0);
}

void	del_listone(t_list **sort)
{
	t_list		*tmp;
	t_dircont	*dc;

	tmp = (*sort)->next ? (*sort)->next : (*sort);
	dc = tmp->content;
	if (dc->name)
		ft_strdel(&dc->name);
	if (dc->type)
		ft_strdel(&dc->type);
	if (dc->path)
		ft_strdel(&dc->path);
	free(*sort);
	*sort = tmp;
}
