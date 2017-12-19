/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <qdegraev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 20:19:15 by qdegraev          #+#    #+#             */
/*   Updated: 2017/12/19 14:43:44 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_concat(int n, ...)
{
	va_list	ap;
	char	*junk;
	char	*res;

	va_start(ap, n);
	res = NULL;
	while (--n >= 0)
	{
		junk = res;
		res = ft_strjoin(res, va_arg(ap, char*));
		if (junk != NULL)
			ft_strdel(&junk);
	}
	va_end(ap);
	return (res);
}
