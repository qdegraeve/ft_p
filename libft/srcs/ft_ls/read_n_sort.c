/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_n_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/01 00:21:03 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/13 15:08:28 by qdegraev         ###   ########.fr       */
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

static char	**print_dir(t_list *sort, t_display *d)
{
	t_dircont	*dc;
	char		**ret;
	int			i;

	i = 1;
	if (!(ret = (char**)malloc(sizeof(char*) * (d->o->folder_size + 3))))
		return (NULL);
	ret[0] = ft_strjoin("total ", ft_itoa(d->total));
	while (sort)
	{
		dc = sort->content;
		if (d->o->l)
			ret[i++] = display_long(dc, d);
		else
			ret[i++] = ft_strdup(dc->name);
		lst_delone(&sort, del_dircont);
	}
	ret[i++] = "EOT";
	ret[i] = NULL;
	return (ret);
}

static void	set_display(t_display *d, t_stat stat, int namlen, char type)
{
	struct group	*gr;
	struct passwd	*pwd;

	pwd = getpwuid(stat.st_uid);
	gr = getgrgid(stat.st_gid);
	if (d->len_max < namlen)
		d->len_max = namlen;
	if (d->link_max < ft_num_len(stat.st_nlink))
		d->link_max = ft_num_len(stat.st_nlink);
	if (pwd && pwd->pw_name && d->owner_max < (int)ft_strlen(pwd->pw_name))
		d->owner_max = ft_strlen(pwd->pw_name);
	else if (!pwd && !pwd->pw_name && d->owner_max < ft_num_len(stat.st_uid))
		d->owner_max = ft_num_len(stat.st_uid);
	if (gr && gr->gr_name && d->group_max < (int)ft_strlen(gr->gr_name))
		d->group_max = ft_strlen(gr->gr_name);
	else if (!gr && !gr->gr_name && d->group_max < ft_num_len(stat.st_gid))
		d->group_max = ft_num_len(stat.st_gid);
	if (d->size_max < ft_num_len(stat.st_size))
		d->size_max = ft_num_len(stat.st_size);
	if (type == 'b' || type == 'c')
		d->sys = 1;
}

static char	**stockdir(char *path, DIR *dir, t_display d)
{
	t_dirent	*fich;
	t_list		*sort;
	t_dircont	dc;

	sort = NULL;
	ft_bzero(&dc, sizeof(dc));
	init_display(&d);
	while ((fich = readdir(dir)))
	{
		if (d.o->a || fich->d_name[0] != '.')
		{
			dc.path = ft_strcmp(path, "/") ? ft_strjoin(path, "/") :
				ft_strdup(path);
			dc.path = ft_cjoin(dc.path, ft_strdup(fich->d_name));
			lstat(dc.path, &dc.stat);
			dc.name = ft_strdup(fich->d_name);
			dc.type = define_type(dc.stat.st_mode);
			set_display(&d, dc.stat, fich->d_namlen, dc.type[0]);
			d.total += dc.stat.st_blocks;
			sort_select(&sort, &dc, sizeof(dc));
			d.o->folder_size++;
		}
	}
	return (print_dir(sort, &d));
}

char		**open_dir(char *av, t_display d)
{
	DIR		*dir;
	char	**ret;

	dir = opendir(av);
	if (dir == NULL)
		return (NULL);
	ret = stockdir(av, dir, d);
	closedir(dir);
	return (ret);
}
