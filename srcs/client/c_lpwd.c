#include "client.h"

int		exec_lpwd(char *command, int sock)
{
	char	*path;

	path = NULL;
	sock = 0;
	if (ft_strcmp(command, "lpwd") != 0)
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	if ((path = getcwd(path, 255)))
	{
		ft_putendl(path);
		ft_strdel(&path);
	}
	else
		return (1);
	return (0);
}
