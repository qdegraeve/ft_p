/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/08 01:52:30 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/14 11:40:02 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	print_size(t_dircont *dc, t_display *d, char **ret)
{
	char	tmp[128];

	if (dc->type[0] == 'c' || dc->type[0] == 'b')
		sprintf(tmp, "%3d,%*d ", major(dc->stat.st_rdev), d->size_max > 4 ?
				d->size_max : 4, minor(dc->stat.st_rdev));
	else if (d->sys)
		sprintf(tmp, "%*lld ", d->size_max > 4 ? d->size_max + 4 : 8
				, dc->stat.st_size);
	else
		sprintf(tmp, "%*lld ", d->size_max, dc->stat.st_size);
	*ret = ft_cjoin(*ret, tmp);
	return ;
}

static void print_time(t_dircont *dc, char **ret)
{
	time_t	t;
	char	*st;
	char	tmp[128];

	t = (dc->stat.st_mtimespec).tv_sec;
	st = ctime(&t);
	sprintf(tmp, "%-.12s ", st + 4);
	*ret = ft_cjoin(*ret, tmp);
}

char		*display_long(t_dircont *dc, t_display *d)
{
	char	*buff;
	int		i;
	char	*ret;
	char	tmp[128] = "";

	ret = NULL;
	i = 0;
	buff = (char*)malloc(255);
	sprintf(tmp, "%-12s", dc->type);
	ret = ft_cjoin(ret, tmp);
	sprintf(tmp, "%*d ", d->link_max, dc->stat.st_nlink);
	ret = ft_cjoin(ret, tmp);
	print_size(dc, d, &ret);
	print_time(dc, &ret);
	sprintf(tmp, "%-s", dc->name);
	ret = ft_cjoin(ret, tmp);
	if (dc->type[0] == 'l' && (i = readlink(dc->path, buff, 255)))
	{
		buff[i] = '\0';
		sprintf(tmp, " -> %s\n", buff);
		ret = ft_cjoin(ret, tmp);
	}
	ft_strdel(&buff);
	return (ret);
}
