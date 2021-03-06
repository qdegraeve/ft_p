/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdegraev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/16 10:08:54 by qdegraev          #+#    #+#             */
/*   Updated: 2016/03/16 10:08:55 by qdegraev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	char		*dest_cpy;
	const char	*src_cpy;
	size_t		i;
	size_t		dest_len;

	dest_cpy = dest;
	src_cpy = src;
	i = size;
	while (i-- != 0 && *dest_cpy != '\0')
		dest_cpy++;
	dest_len = dest_cpy - dest;
	i = size - dest_len;
	if (i == 0)
		return (dest_len + ft_strlen(src_cpy));
	while (*src_cpy != '\0')
	{
		if (i != 1)
		{
			*dest_cpy++ = *src_cpy;
			i--;
		}
		src_cpy++;
	}
	*dest_cpy = '\0';
	return (dest_len + (src_cpy - src));
}
