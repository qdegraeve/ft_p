#include "server.h"

int			exec_where(const char **cmd, const int csock)
{
	char	*path;

	path = NULL;
	if (cmd)
	{
		path = getcwd(path, 255);
		send(csock, path, ft_strlen(path), 0);
		ft_strdel(&path);
	}
	return (0);
}
