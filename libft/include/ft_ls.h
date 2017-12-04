/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/24 16:24:48 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/18 10:18:14 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "libft.h"

# include <sys/ioctl.h>

/*
** opendir readdir closedir
*/
# include <sys/types.h>
# include <dirent.h>

/*
** stat lstat
*/
# include <sys/stat.h>
# include <unistd.h>

# include <pwd.h>
# include <grp.h>
# include <time.h>

/*
** errno
*/
# include <errno.h>

/*
** perror
*/
# include <stdio.h>

# include <stdlib.h>

# include <ncurses.h>
# include <curses.h>

typedef struct dirent	t_dirent;
typedef struct stat		t_stat;

typedef struct		s_options
{
	int				name;
	char			l;
	char			a;
	char			one;
	char			color;
	int				folder_size;
}					t_options;

typedef struct		s_dircont
{
	char			*path;
	char			*name;
	char			*type;
	t_stat			stat;
}					t_dircont;

typedef struct		s_display
{
	int				sys;
	int				len_max;
	int				link_max;
	int				owner_max;
	int				group_max;
	int				size_max;
	int				total;
	t_options		*o;
}					t_display;

/*
** options
*/
void				init_options(t_options *o);
void				set_options(char option, t_options *o);
int					check_options(char **av, t_options *o);

/*
** read_n_sort
*/
char				**open_dir(char *av, t_display d);

/*
** print
*/
char				*display_long(t_dircont *dc, t_display *d);

/*
** file type
*/
void				define_permission(char *perm, mode_t st_mode);
char				*define_type(mode_t st_mode);

/*
** lists tools
*/
void				sort_select(t_list **dir, void *content, size_t cont_size);
void				sort_list(t_list **dir, int o,
		int (*sort)(t_dircont *, void *, int));
int					sort_ascii(t_dircont *c1, void *c2);

/*
** Mr Proper
*/
void				ft_lst_sortinsert(t_list **beg_lst, t_list *new,
		int (*sort)(t_dircont *, void *));
void				ft_lst_insert(t_list **lst, t_list *in);
void				lst_delone(t_list **lst, void (*del)(void *));
void				del_dircont(void *to_del);
void				del_string(void *to_del);

/*
** tools
*/
int					ft_num_len(size_t num);
int					ft_isnavdir(char *name);
int					ft_ishidden(char *name);

char				**ft_ls(char **args);

#endif
