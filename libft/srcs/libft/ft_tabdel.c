/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <qdegraev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 20:20:02 by qdegraev          #+#    #+#             */
/*   Updated: 2017/12/19 14:44:22 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_tabdel(char ***as)
{
	int	i;
	int	len;

	if (*as == NULL)
		return ;
	i = 0;
	len = ft_tablen(*as);
	while (i < len && (*as)[i] != NULL)
	{
		ft_strdel(&((*as)[i]));
		i++;
	}
	free(*as);
	*as = NULL;
	return ;
}
