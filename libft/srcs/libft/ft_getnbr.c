/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 11:49:34 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/16 09:48:12 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_getnbr(char *str)
{
	int nb;
	int i;

	i = 0;
	nb = 0;
	if (str[0] == '-')
		return (-ft_getnbr(&str[1]));
	while (ft_isdigit(str[i]) == 1)
	{
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	return (nb);
}
