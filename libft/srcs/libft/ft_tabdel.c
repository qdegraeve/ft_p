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
