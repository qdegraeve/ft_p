/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/08 16:18:40 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/14 11:36:53 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	init_options(t_options *o)
{
	o->name = 0;
	o->l = 0;
	o->a = 0;
	o->color = 0;
	o->folder_size = 0;
}

void	illegal(char option, t_options *o)
{
	if (option == 'G')
		o->color = 1;
	else
	{
		ft_putstr_fd("ls: illegal option -- ", 2);
		ft_putchar_fd(option, 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1]", 2);
		ft_putstr_fd(" [file ...]\n", 2);
		exit(EXIT_FAILURE);
	}
}

void	set_options(char option, t_options *o)
{
	if (option == 'l')
	{
		o->l = 1;
		o->one = 0;
	}
	else if (option == 'a')
		o->a = 1;
	else
		illegal(option, o);
}

int		check_options(char **av, t_options *o)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	while (av[i] && av[i][0] == '-')
	{
		if (ft_strcmp(av[i], "--") == 0)
			return (i + 1);
		else if (av[i][1] == '\0')
			return (i);
		j = 1;
		while (av[i][j])
		{
			set_options(av[i][j], o);
			j++;
		}
		i++;
	}
	return (i);
}
