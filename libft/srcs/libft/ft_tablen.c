/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tablen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <qdegraev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 20:20:04 by qdegraev          #+#    #+#             */
/*   Updated: 2017/12/19 14:44:11 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_tablen(char **tabl)
{
	size_t	i;

	i = 0;
	while (tabl != NULL && tabl[i] != NULL)
		i++;
	return (i);
}
