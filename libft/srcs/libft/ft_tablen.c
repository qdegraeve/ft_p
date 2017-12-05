#include "libft.h"

size_t		ft_tablen(char **tabl)
{
	size_t	i;

	i = 0;
	while (tabl != NULL && tabl[i] != NULL)
		i++;
	return (i);
}