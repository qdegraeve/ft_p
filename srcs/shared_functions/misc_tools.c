#include "ftp.h"

char	*extract_from_path(char *path)
{
	int		i;

	if (!path)
		return (NULL);
	i = ft_strlen(path);
	while (i >= 0 && path[i] != '/')
		i--;
	return (path + i + 1);
}
